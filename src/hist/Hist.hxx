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
#include <set>
#include <TFile.h>
#include <TTree.h>
#include <TTreeFormula.h>
#include <TROOT.h>
#include <TError.h>
#include <TObjArray.h>
#include "Formula.hxx"
#include "hist/Visitor.hxx"
#include "hist/Manager.hxx"
#include "utils/Error.hxx"
#include "utils/LockingPointer.hxx"
#include "utils/nocopy.h"


namespace rb
{
  namespace hist
  {
    typedef std::set<rb::hist::Base*> Container_t;
    struct StopAddDirectory
    {
      StopAddDirectory() { TH1::AddDirectory(false); }
      void BackOn() { TH1::AddDirectory(true); }
      ~StopAddDirectory() { BackOn(); }
    };
    struct LockOnConstruction
    {
      Bool_t kIsLocked;
      rb::Mutex* fMutex;
      LockOnConstruction(rb::Mutex* mutex = 0): kIsLocked(true), fMutex(mutex) {
	if(!fMutex) Lock_TThread();
	else        fMutex->Lock();
      }
      void Unlock() {
	if(!fMutex) UnLock_TThread();
	else        fMutex->UnLock();
	kIsLocked = false;
      }
      ~LockOnConstruction() {
	if(kIsLocked) Unlock();
      }
    };

    /// Rootbeer Base histogram class.

    //! This class extends the stock ROOT TH*D histograms to allow additional functionality.
    //! The main thing that is added is the ability to associate each histogram instance directly
    //! with data values (using TTreeFormula) and then automatically Fill itself with those values.
    //! The class is also (supposed to be) thread-safe, since the aforementioned filling typically
    //! happens in a background thread while the user can still interact with the histogram in CINT.
    //! Thread safety is achieved by marking any shared class members as \c volatile and then accessing
    //! them through the RAII mutex locking LockingPointer class. Mainly for this reason, rb::hist::Base wraps
    //! a stock ROOT histogram rather than inheriting from it, so that we can basically treat the entire
    //! wrapped histogram as a critical object shared between threads. Wrapping also allows the cration of
    //! just one rb::hist::Base class rather than one for each dimension, since we can simply polymorphically cast
    //! the wrapped TH1* to the right type in the constructor.
    class Base : public TNamed
    {
      /// Locks a mutex before be get to TH1D creation in the constructor
      hist::LockOnConstruction fLockOnConstruction;
      /// Stops addition of local histogram to gDirectory
      hist::StopAddDirectory fStopAdd;

    protected:
      /// Number of dimensions (axes).
      const UInt_t kDimensions;

      /// The histogram manager responsible for this instance
      hist::Manager* const fManager;

      /// Clone of the internal histogram.
      //! This is basically the only thing that CINT users can access, via the GetHist() function.
      //! The reason for doing it this way is thread safety. By only allowing CINT users access to
      //! a copy of fHistogram (created within a mutex lock), we ensure that there will never be any
      //! conflicts between the main thread and others that can modify the internal histogram.
      boost::scoped_ptr<TH1> fHistogramClone;

      /// Default title, set from the parameters and gate condition.
      std::string kDefaultTitle;

      /// Use the default title as the actual title?
      Bool_t kUseDefaultTitle;

      /// Directory owning this rb::hist::Base instance.
      TDirectory* fDirectory;

      /// Wrapper for the TTreeFormulae to evaluate the parameter and gate values.
      rb::TreeFormulae fFormulae;

      /// \brief Internal histogram variant.
      //! \details Variant class covers all possible dimensions from 1-3 in one object.
      HistVariant fHistVariant;

      /// Constructor (1d)
      Base(const char* name, const char* title, const char* param, const char* gate,
	   hist::Manager* manager, Int_t event_code,
	   Int_t nbinsx, Double_t xlow, Double_t xhigh);

      /// Constructor (2d)
      Base(const char* name, const char* title, const char* param, const char* gate, 
	   hist::Manager* manager, Int_t event_code,
	   Int_t nbinsx, Double_t xlow, Double_t xhigh,
	   Int_t nbinsy, Double_t ylow, Double_t yhigh);

      /// Constructor (3d)
      Base(const char* name, const char* title, const char* param, const char* gate, 
	   hist::Manager* manager, Int_t event_code,
	   Int_t nbinsx, Double_t xlow, Double_t xhigh,
	   Int_t nbinsy, Double_t ylow, Double_t yhigh,
	   Int_t nbinsz, Double_t zlow, Double_t zhigh);

    public:
      /// Default constructor.
      //! Does nothing, just here to make rootcint happy.
      Base() : kDimensions(0), fManager(0) {}

    private:
      /// Destruction function, acts like a normal dstructor
      virtual void Destruct() {}

    public:
      /// Destructor
      /// \warning Derived classes should not implement a destructor. If additional
      /// memory de-allocation is needed in addition to what's done here, they should
      /// instead override the virtual Destruct() member function.
      virtual ~Base() {
	fManager->Remove(this); // locks TTHREAD_GLOBAL_MUTEX while running
	Destruct();
      }

      /// Function to change the histogram gate.
      //! Updates \c fGate to reflect the new gate formula. Returns 0 if successful,
      //!  -1 if \c newgate isn't valid. In case of invalid \c newgate, the histogram
      //!  gate condition remains unchanged.
      virtual Int_t Regate(const char* newgate);

      /// Fill the histogram from its internal parameter value(s).
      //! \note This function locks the mutex passed to the histogram in the constructor
      //! (the mutex which protects the data).  If you are calling from an already locked
      //! section of code, use FillUnlocked() instead.
      Int_t Fill();

#ifndef __MAKECINT__
      /// Unlocked version of Fill().
      Int_t FillUnlocked();
#endif

      /// Returns a copy of fHistogram.
      //! This is so that users can get access to the histogram data in CINT, thus allowing higher-order analysis
      //! such as fitting, but without having to worry about thread-related issues. Since the histogram returned is
      //! a /emph copy of fHistogram, other threads can happily continue to alter fHistogram behind the scenes while
      //! users do analysis on the returned copy.
      TH1* GetHist();

      /// Clear function, zeros-out all axes of the internal histogram
      virtual void Clear() { visit::hist::Clear::Do(fHistVariant); }

      /// Return the number of dimensions.
      UInt_t GetNdimensions() { return kDimensions; }

      /// Return the gate argument
      std::string GetGate() { return fFormulae.Get(GATE); }

      /// Return the parameter name associated with the specified axis.
      std::string GetParam(Int_t axis) { return fFormulae.Get(axis); }

      //   FORWARDS OF TH1 FUNCTIONS     //
      /// Draw function.
      //! \Note that this draws fHistogram directly, not a copy (to that it can still continue to be auto-updated
      //! by the rb::canvas functions).
      void Draw(Option_t* option = "") {
	visit::hist::DoMember(fHistVariant, &TH1::Draw, option);
      }
      /// Set line color.
      void SetLineColor(Color_t lcolor) {
	visit::hist::DoMember(fHistVariant, &TH1::SetLineColor, lcolor);
      }
      /// Set marker color.
      void SetMarkerColor(Color_t mcolor) {
	visit::hist::DoMember(fHistVariant, &TH1::SetMarkerColor, mcolor);
      }
      // ******************************* //
    private:
      /// Set name and title
      virtual void Init(const char* name, const char* title, const char* param, const char* gate);
      /// Prevent assigmnent
      Base& operator= (const Base& other) { return *this; }
      /// Prevent copying
      Base(const Base& other) : kDimensions(other.kDimensions), fManager(other.fManager) {}
      /// Internal function to fill the histogram.
      //! Called from the public Fill() and FillAll(), does not do any mutex locking,
      //! instead relies on being passed already locked components.
      virtual Int_t DoFill(const std::vector<Double_t>& params, Double_t gate);
    public:
      friend class rb::hist::Manager;
      ClassDef(rb::hist::Base, 0);
    };

    /// One-dimensional derived implementation
    class D1: public Base
    {
    public:
      D1 (const char* name, const char* title, const char* param, const char* gate,
	  hist::Manager* manager, Int_t event_code,
	  Int_t nbinsx, Double_t xlow, Double_t xhigh):
	Base(name, title, param, gate, manager, event_code, nbinsx, xlow, xhigh) {}
      ClassDef(rb::hist::D1, 0);
    };
    /// Two-dimensional derived implementaion
    class D2: public Base
    {
    public:
      D2 (const char* name, const char* title, const char* param, const char* gate,
	  hist::Manager* manager, Int_t event_code,
	  Int_t nbinsx, Double_t xlow, Double_t xhigh,
	  Int_t nbinsy, Double_t ylow, Double_t yhigh):
	Base(name, title, param, gate, manager, event_code, nbinsx, xlow, xhigh, nbinsy, ylow, yhigh) {}
      ClassDef(rb::hist::D2, 0);
    };
    /// Three-dimensional derived implementation
    class D3: public Base
    {
    public:
      D3 (const char* name, const char* title, const char* param, const char* gate,
	  hist::Manager* manager, Int_t event_code,
	  Int_t nbinsx, Double_t xlow, Double_t xhigh,
	  Int_t nbinsy, Double_t ylow, Double_t yhigh,
	  Int_t nbinsz, Double_t zlow, Double_t zhigh):
	Base(name, title, param, gate, manager, event_code, nbinsx, xlow, xhigh, nbinsy, ylow, yhigh, nbinsz, zlow, zhigh) {}
      ClassDef(rb::hist::D3, 0);
    };
  } // namespace hist
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
//     //! Same idea as for rb::hist::Base, but implemented as needed for a summary histogram.
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
//     //! Same idea as for rb::hist::Base, but implemented as needed for a gamma histogram.
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
//   // Create rb::hist::Base instance
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

