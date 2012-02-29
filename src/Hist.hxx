//! \file Hist.hxx
//! \brief Defines the histogram class used in <tt>ROOTBEER</tt>.
#ifndef HIST_HXX
#define HIST_HXX
#include <cassert>
#include <stdexcept>
#include <memory>
#include <string>
#include <sstream>
#include <vector>
#include <bitset>
#include <TFile.h>
#include <TTree.h>
#include <TTreeFormula.h>
#include <TROOT.h>
#include <TError.h>
#include <TObjArray.h>
#include "utils/Error.hxx"
#include "utils/LockingPointer.hxx"
#include "Formula.hxx"
#include "Visitor.hxx"

namespace rb
{
  namespace hist
  {
    struct StopAddDirectory
    {
      StopAddDirectory() { TH1::AddDirectory(false); }
      void BackOn() { TH1::AddDirectory(true); }
      ~StopAddDirectory() { BackOn(); }
    };
  }

  /// Rootbeer Histogram class.

  //! This class extends the stock ROOT TH*D histograms to allow additional functionality.
  //! The main thing that is added is the ability to associate each histogram instance directly
  //! with data values (using TTreeFormula) and then automatically Fill itself with those values.
  //! The class is also (supposed to be) thread-safe, since the aforementioned filling typically
  //! happens in a background thread while the user can still interact with the histogram in CINT.
  //! Thread safety is achieved by marking any shared class members as \c volatile and then accessing
  //! them through the RAII mutex locking LockingPointer class. Mainly for this reason, rb::Hist wraps
  //! a stock ROOT histogram rather than inheriting from it, so that we can basically treat the entire
  //! wrapped histogram as a critical object shared between threads. Wrapping also allows the cration of
  //! just one rb::Hist class rather than one for each dimension, since we can simply polymorphically cast
  //! the wrapped TH1* to the right type in the constructor.
  class Hist : public TNamed
  {
    /// Stops addition of local histogram to gDirectory
    hist::StopAddDirectory fStopAdd;
  protected:
    /// Mutex, protects histogram internals
    rb::Mutex fMutex;
    /// Number of dimensions (axes).
    const UInt_t kDimensions;
    /// Default title, set from the parameters and gate condition.
    std::string kDefaultTitle;
    /// Use the default title as the actual title?
    Bool_t kUseDefaultTitle;
    /// Directory owning this rb::Hist instance.
    TDirectory* fDirectory;
    /// Wrapper for the TTreeFormulae to evaluate the parameter and gate values.
    formula::HistWrapper fGateParams;
    /// \brief Internal histogram variant.
    //! \details Variant class covers all possible dimensions from 1-3 in one object.
    HistVariant fHistVariant;
    /// Clone of the internal histogram.
    //! This is basically the only thing that CINT users can access, via the GetHist() function.
    //! The reason for doing it this way is thread safety. By only allowing CINT users access to
    //! a copy of fHistogram (created within a mutex lock), we ensure that there will never be any
    //! conflicts between the main thread and others that can modify the internal histogram.
    boost::scoped_ptr<TH1> fHistogramClone;
  public:
    /// Constructor (1d)
    Hist(const char* name, const char* title, const char* param, const char* gate, 
	 TTree* const tree, rb::Mutex* const data_mutex,
	 Int_t nbinsx, Double_t xlow, Double_t xhigh) :
      fMutex(false), kDimensions(1), fHistogramClone(0),
      fGateParams(tree, data_mutex, 1, param, gate),
      fHistVariant(TH1D(name, title, nbinsx, xlow, xhigh)) {
      Init(name, title, param, gate);
    }
    /// Constructor (2d)
    Hist(const char* name, const char* title, const char* param, const char* gate, 
	 TTree* const tree, rb::Mutex* const data_mutex,
	 Int_t nbinsx, Double_t xlow, Double_t xhigh,
	 Int_t nbinsy, Double_t ylow, Double_t yhigh) :
      fMutex(false), kDimensions(2), fHistogramClone(0),
      fGateParams(tree, data_mutex, 2, param, gate),
      fHistVariant(TH2D(name, title, nbinsx, xlow, xhigh, nbinsy, ylow, yhigh)) {
      Init(name, title, param, gate);
    }
    /// Constructor (3d)
    Hist(const char* name, const char* title, const char* param, const char* gate, 
	 TTree* const tree, rb::Mutex* const data_mutex,
	 Int_t nbinsx, Double_t xlow, Double_t xhigh,
	 Int_t nbinsy, Double_t ylow, Double_t yhigh,
	 Int_t nbinsz, Double_t zlow, Double_t zhigh) :
      fMutex(false), kDimensions(3), fHistogramClone(0),
      fGateParams(tree, data_mutex, 3, param, gate),
      fHistVariant(TH3D(name, title, nbinsx, xlow, xhigh, nbinsy, ylow, yhigh, nbinsz, zlow, zhigh)) {
      Init(name, title, param, gate);
    }
    /// Default constructor.
    //! Does nothing, just here to make rootcint happy.
    Hist() : kDimensions(0) {}

    void Delete(Option_t* option = "") { delete this; }
  protected:
    /// Destructor
    //! Free resorces allocted to TTreeFormulae, remove this from fgList.
    virtual ~Hist();
  public:
    /// Deletes all entries of \c Hist::fgList
    static void DeleteAll();

    /// Function to change the histogram gate.
    //! Updates \c fGate to reflect the new gate formula. Returns 0 if successful,
    //!  -1 if \c newgate isn't valid. In case of invalid \c newgate, the histogram
    //!  gate condition remains unchanged.
    virtual Int_t Regate(const char* newgate);

    /// Fill the histogram from its internal parameter value(s).
    Int_t Fill();

    /// Returns a copy of fHistogram.
    //! This is so that users can get access to the histogram data in CINT, thus allowing higher-order analysis
    //! such as fitting, but without having to worry about thread-related issues. Since the histogram returned is
    //! a /emph copy of fHistogram, other threads can happily continue to alter fHistogram behind the scenes while
    //! users do analysis on the returned copy.
    TH1* GetHist();

    /// Clear function, zeros-out all axes of the internal histogram
    virtual void Clear() { visit::hist::Clear::Do(&fMutex, fHistVariant); }

    /// Return the number of dimensions.
    UInt_t GetNdimensions() { return kDimensions; }

    /// Return the gate argument
    std::string GetGate() { fGateParams.Get(GATE); }

    /// Return the parameter name associated with the specified axis.
    std::string GetParam(Int_t axis) { return fGateParams.Get(axis); }

    //   FORWARDS OF TH1 FUNCTIONS     //
    /// Draw function.
    //! \Note that this draws fHistogram directly, not a copy (to that it can still continue to be auto-updated
    //! by the rb::canvas functions).
    void Draw(Option_t* option = "") {
      visit::hist::DoMember(&fMutex, fHistVariant, &TH1::Draw, option);
    }
    /// Set line color.
    void SetLineColor(Color_t lcolor) {
      visit::hist::DoMember(&fMutex, fHistVariant, &TH1::SetLineColor, lcolor);
    }
    /// Set marker color.
    void SetMarkerColor(Color_t mcolor) {
      visit::hist::DoMember(&fMutex, fHistVariant, &TH1::SetMarkerColor, mcolor);
    }
    // ******************************* //
  private:
    /// Set name and title
    virtual void Init(const char* name, const char* title, const char* param, const char* gate);
    /// Prevent assigmnent
    Hist& operator= (const Hist& other) {}
    /// Prevent copying
    Hist(const Hist& other) : kDimensions(other.kDimensions) {}
    /// Internal function to fill the histogram.
    //! Called from the public Fill() and FillAll(), does not do any mutex locking,
    //! instead relies on being passed already locked components.
    virtual Int_t DoFill(HistVariant& hst, boost::scoped_ptr<TTreeFormula>& gate, boost::scoped_ptr<TTreeFormula>* params);
  public:
    ClassDef(Hist, 0);
  };


} // namespace rb
// #if 0
//   /// Summary histogram class.

//   //! A "summary" histogram is a type of 2d histogram in which each bin along one of the axes ("parameter axis") corresponds
//   //! to a specific parameter. The other axis ("summary axis") corresponds to the parameter values, and the z axis corresponds
//   //! to counts. In other wirds, the 1d projection of each bin along the first axis would be identical to
//   //! a 1d histogram for the corresponding parameter.  Orientation can either be hirozontal or vertical. Horizontal
//   //! means that the parameter axis is the y axis (so the parameter values extend horizontally), vertical means it's
//   //! the x axis (parameter values extend vertically).
//   class SummaryHist : public Hist
//   {
//   protected:
//     //! Histogram orientation.
//     //! 0 = vertical, 1 = horizontal
//     Int_t kOrient;

//     //! Number of parameters summarized.
//     Int_t nPar;

//     //! Constructor
//     SummaryHist(const char* name, const char* title, const char* params, const char* gate, const char* orient, TTree* tree);

//     //! Internal filling function.
//     //! Same idea as for rb::Hist, but implemented as needed for a summary histogram.
//     virtual Int_t DoFill(Types& hst, TTreeFormula* gate, std::vector<TTreeFormula*>& params);

//   public:
//     //! Public creation function.
//     //! \param nbins Number of bins along the summary axis
//     //! \param low Low bin along the summary axis
//     //! \param high High bin along the summary axis
//     //! \param paramList List of all the parameters to be displayed separated by semicolons.
//     //!        Arrays can also be specified as (e.g. from index 0 to index 15 inclusive) \c someArray[0-15]
//     //! \param orientation "v" = vertical "h" = horizontal, not case sensitive.
//     // friend void rb::hist::NewSummary(const char* name, const char* title,
//     // 				     Int_t nbins, Double_t low, Double_t high,
//     // 				     const char* paramList,  const char* gate = "",
//     // 				     const char* orientation = "v");

//     //! Return the number of parameters.
//     Int_t GetNPar() {
//       return nPar;
//     }

//     //! CINT ClassDef macro.
//     ClassDef(SummaryHist, 0);
//   };

//   /// Gamma histogram class.

//   //! A "gamma" histogram is a type of histogram displaying multiple parameters at once. It appears as a normal
//   //! 1d or 2d histogram, but the number of counts per bin increments whenever <i>any</i> of the specified
//   //! parameters has a value in that bin.
//   class GammaHist : public Hist
//   {
//   protected:
//     //! Number of parameters.
//     Int_t nPar;

//     //! Constructor
//     GammaHist(const char* name, const char* title, const char* params, const char* gate, Int_t ndimensions, TTree* tree, Set_t* set);

//   public:
//     //! Initialize function
//     //! Helper function called by New().
//     static Bool_t GInitialize(const char* name, const char* title,
// 			      const char* param, const char* gate,
// 			      UInt_t ndim, TTree* tree, Set_t* set,
// 			      Int_t nbinsx, Double_t xlow, Double_t xhigh,
// 			      Int_t nbinsy = 0, Double_t ylow = 0, Double_t yhigh = 0,
// 			      Int_t nbinsz = 0, Double_t zlow = 0, Double_t zhigh = 0);

//   protected:
//     //! Internal filling function.
//     //! Same idea as for rb::Hist, but implemented as needed for a gamma histogram.
//     virtual Int_t DoFill(TH1* hst, TTreeFormula* gate, std::vector<TTreeFormula*>& params);

//   public:
//     // //! Public creation function for 1d.
//     // //! Looks like the normal 1d creation function, but the multiple parameters are
//     // //! delimited with a semicolon.
//     // friend void rb::hist::NewGamma(const char* name, const char* title,
//     // 				   Int_t nbinsx, Double_t xlow, Double_t xhigh,
//     // 				   const char* params,  const char* gate = "");

//     // //! Public creation function for 2d.
//     // //! Looks like the normal 2d creation function, but the multiple parameters are
//     // //! delimited with a semicolon.  The standard prescription applies for dividing x and y axes: Y:X.
//     // friend void rb::hist::NewGamma(const char* name, const char* title,
//     // 				   Int_t nbinsx, Double_t xlow, Double_t xhigh,
//     // 				   Int_t nbinsy, Double_t ylow, Double_t yhigh,
//     // 				   const char* params,  const char* gate = "");

//     //! CINT ClassDef macro.
//     ClassDef(GammaHist, 0);
//   };

//   /// Bitmask histogram class.

//   //! A bitmask histogram displays the true bits in a parameter.  For each event,
//   //! the bin corresponding to a given bit in a word increments if that bit is set to 1.
//   //! This is a template class, with the template argument being the number of bits to
//   //! display.
//   //! \note Currently, CINT dictionaries are only generated for the following numbers of
//   //! bits: 1, 2, 4, 8, 16, 32, and 64. If you need others, you'll need to add
//   //! \code
//   //! #pragma link C++ class rb::BitHist<N>+; // N is the number of bits you want
//   //! \endcode
//   //! to HistLinkdef.h

//   template <Int_t NBITS>
//   class BitHist : public Hist
//   {
//   protected:
//     //! Constructor
//     BitHist(const char* name, const char* title, const char* param, const char* gate, TTree* tree, Set_t* set);

//   public:
//     //! New() helper function
//     static Bool_t BitInitialize(const char* name, const char* title, const char* param, const char* gate, TTree* tree, Set_t* set);

//   protected:
//     //! Internal filling function
//     virtual Int_t DoFill(TH1* hst, TTreeFormula* gate, std::vector<TTreeFormula*>& params);

//   public:
//     // //! Creation function
//     // friend void rb::hist::NewBits<NBITS>(const char* name, const char* title, const char* param, const char* gate);

//     ClassDef(BitHist, 0);
//   };
// }


// #ifndef __MAKECINT__ // Template implementations

// template <Int_t NBITS>
// rb::BitHist<NBITS>::BitHist(const char* name, const char* title, const char* param, const char* gate, TTree* tree, Set_t* set) :
//   Hist(name, title, param, gate, 1, tree, set) { }

// // template <Int_t NBITS>
// // inline void rb::BitHist<NBITS>::New(const char* name, const char* title, const char* param, const char* gate) {
// //   BitInitialize(name, title, param, gate);
// // }

// template <Int_t NBITS>
// Int_t rb::BitHist<NBITS>::DoFill(TH1* hst, TTreeFormula* gate, std::vector<TTreeFormula*>& params) {
//   if(!gate->EvalInstance()) return 0;

//   Int_t ret = 0;
//   std::bitset<NBITS> bits (params[0]->EvalInstance());
//   for(Int_t i=0; i< NBITS; ++i) {
//     if (bits[i]) {
//       static_cast<TH1D*>(hst)->Fill(i);
//       ++ret;
//     }
//   }
//   return ret;
// }

// template <Int_t NBITS>
// Bool_t rb::BitHist<NBITS>::BitInitialize(const char* name, const char* title, const char* param, const char* gate, TTree* tree, Set_t* set) {
//   // Create rb::Hist instance
//   rb::BitHist<NBITS>* _this = new rb::BitHist<NBITS>(name, title, param, gate, tree, set);
//   if(!_this->kConstructorSuccess) return kFALSE;

//   // Set internal histogram
//   //! \note The histogram isn't accessable to any other threads until we add it to
//   //! fgList, so it's safe to access the critical elements via a non-locking pointer.
//   LockFreePointer<CriticalElements> unlocked_critical(_this->fCritical);

//   Bool_t successfulHistCreation = kTRUE;
//   TH1::AddDirectory(kFALSE);
//   unlocked_critical->fHistogram =
//     new TH1D(_this->fName, _this->fTitle, NBITS, 0, NBITS);

//   std::string xtitle = unlocked_critical->fParams.at(0)->GetExpFormula().Data();
//   xtitle += " [bits]";
//   unlocked_critical->fHistogram->GetXaxis()->SetTitle(xtitle.c_str());
//   TH1::AddDirectory(kTRUE);

//   // Add to collections
//   // LockingPointer<List_t> hlist(fgList(), &fMutex);
//   // if(successfulHistCreation) {
//   //   hlist->push_back(_this);

//   rb::ScopedLock lock(NULL);
//     if(gDirectory) {
//       _this->fDirectory = gDirectory;
//       _this->fDirectory->Append(_this, kTRUE);
//     }
//   }
//   return successfulHistCreation;
// }
// #endif // #if 0
// #endif // $ifndef __MAKECINT__


#endif

