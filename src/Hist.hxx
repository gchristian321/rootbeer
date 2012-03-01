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
#include "utils/Error.hxx"
#include "utils/LockingPointer.hxx"
#include "Formula.hxx"
#include "Visitor.hxx"



namespace rb
{
  class Hist;
  namespace hist
  {
    typedef std::set<rb::Hist*> Container_t;
    struct StopAddDirectory
    {
      StopAddDirectory() { TH1::AddDirectory(false); }
      void BackOn() { TH1::AddDirectory(true); }
      ~StopAddDirectory() { BackOn(); }
    };
    struct LockOnConstruction
    {
      Bool_t kIsLocked;
      LockOnConstruction(): kIsLocked(true) { TThread::Lock(); }
      void Unlock() { TThread::UnLock(); kIsLocked = false; }
      ~LockOnConstruction() { if(kIsLocked) TThread::UnLock(); }
    };

    /// \brief Wraps a container of all histograms registered to this event type.
    //! \details Also takes care of functions for Adding and Deleting histograms.
    class Manager
    {
    public: ////    private:
      //! Reference to Event::fMutex
      rb::Mutex& fMutex;
      TTree* fTree;
      //! Container of pointers to histograms registered to this event type.
      volatile Container_t fSet;
      //! Fill all histograms in fSet
      void FillAll();
      //! Add a histogram to fSet
      void Add(rb::Hist* hist);
    public:
      //! Set fMutex
      Manager(TTree* tree, rb::Mutex& fMutex);
      //! Deletes all entries in fSet
      ~Manager();
      //! Create a new 1d histogram and add to fSet
      template<typename T>
      rb::Hist* Create(const char* name, const char* title, const char* param, const char* gate, 
		  Int_t nbinsx, Double_t xlow, Double_t xhigh) {
	rb::Hist* hist =
	  new T(name, title, param, gate, this, nbinsx, xlow, xhigh);
	Add(hist);
      }
      //! Create a new 2d histogram and add to fSet
      template<typename T>
      rb::Hist* Create(const char* name, const char* title, const char* param, const char* gate, 
		  Int_t nbinsx, Double_t xlow, Double_t xhigh,
		  Int_t nbinsy, Double_t ylow, Double_t yhigh) {
	rb::Hist* hist =
	  new T(name, title, param, gate, this, nbinsx, xlow, xhigh, nbinsy, ylow, yhigh);
	Add(hist);
      }
      //! Create a new 3d histogram and add to fSet
      template<typename T>
      rb::Hist* Create(const char* name, const char* title, const char* param, const char* gate, 
		  Int_t nbinsx, Double_t xlow, Double_t xhigh,
		  Int_t nbinsy, Double_t ylow, Double_t yhigh,
		  Int_t nbinsz, Double_t zlow, Double_t zhigh) {
	rb::Hist* hist =
	  new T(name, title, param, gate, this, nbinsx, xlow, xhigh, nbinsy, ylow, yhigh, nbinsz, zlow, zhigh);
	Add(hist);
      }
      //! Remove a histogram and from fSet
      void Remove(rb::Hist* hist);
      //! Delete all histogram instances
      void DeleteAll();
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
    /// Locks a mutex before be get to TH1D creation in the constructor
    hist::LockOnConstruction fLockOnConstruction;
    /// Stops addition of local histogram to gDirectory
    hist::StopAddDirectory fStopAdd;

  protected:
    /// The histogram manager responsible for this instance
    hist::Manager* const fManager;
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

    /// Constructor (1d)
    Hist(const char* name, const char* title, const char* param, const char* gate,
	 hist::Manager* manager,
	 Int_t nbinsx, Double_t xlow, Double_t xhigh) :
      kDimensions(1), fHistogramClone(0), fManager(manager),
      fGateParams(fManager->fTree, &fManager->fMutex, 1, param, gate),
      fHistVariant(TH1D(name, title, nbinsx, xlow, xhigh)) {
      Init(name, title, param, gate);
      fLockOnConstruction.Unlock();
    }
    /// Constructor (2d)
    Hist(const char* name, const char* title, const char* param, const char* gate, 
	 hist::Manager* manager,
	 Int_t nbinsx, Double_t xlow, Double_t xhigh,
	 Int_t nbinsy, Double_t ylow, Double_t yhigh) :
      kDimensions(2), fHistogramClone(0), fManager(manager),
      fGateParams(fManager->fTree, &fManager->fMutex, 2, param, gate),
      fHistVariant(TH2D(name, title, nbinsx, xlow, xhigh, nbinsy, ylow, yhigh)) {
      Init(name, title, param, gate);
      fLockOnConstruction.Unlock();
    }
    /// Constructor (3d)
    Hist(const char* name, const char* title, const char* param, const char* gate, 
	 hist::Manager* manager,
	 Int_t nbinsx, Double_t xlow, Double_t xhigh,
	 Int_t nbinsy, Double_t ylow, Double_t yhigh,
	 Int_t nbinsz, Double_t zlow, Double_t zhigh) :
      kDimensions(3), fHistogramClone(0), fManager(manager),
      fGateParams(fManager->fTree, &fManager->fMutex, 3, param, gate),
      fHistVariant(TH3D(name, title, nbinsx, xlow, xhigh, nbinsy, ylow, yhigh, nbinsz, zlow, zhigh)) {
      Init(name, title, param, gate);
      fLockOnConstruction.Unlock();
    }
  public:
    /// Default constructor.
    //! Does nothing, just here to make rootcint happy.
    Hist() : kDimensions(0), fManager(0) {}

  private:
    /// Destruction function, acts like a normal dstructor
    virtual void Destruct() {}

  public:
    /// Destructor
    /// \warning Derived classes should not implement a destructor. If additional
    /// memory de-allocation is needed in addition to what's done here, they should
    /// instead override the virtual Destruct() member function.
    virtual ~Hist() {
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
    std::string GetGate() { fGateParams.Get(GATE); }

    /// Return the parameter name associated with the specified axis.
    std::string GetParam(Int_t axis) { return fGateParams.Get(axis); }

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
    Hist& operator= (const Hist& other) {}
    /// Prevent copying
    Hist(const Hist& other) : kDimensions(other.kDimensions), fManager(other.fManager) {}
    /// Internal function to fill the histogram.
    //! Called from the public Fill() and FillAll(), does not do any mutex locking,
    //! instead relies on being passed already locked components.
    virtual Int_t DoFill(const std::vector<Double_t>& params, Double_t gate);
  public:
    friend class rb::hist::Manager;
    ClassDef(Hist, 0);
  };

  namespace {
    struct HistFill { Int_t operator() (rb::Hist* const& hist) {
      return hist->FillUnlocked();
    } } fill_hist;
  }
  inline void hist::Manager::FillAll() {
    LockingPointer<hist::Container_t> pSet(fSet, TTHREAD_GLOBAL_MUTEX);
    std::for_each(pSet->begin(), pSet->end(), fill_hist);
  }
  inline hist::Manager::Manager(TTree* tree, rb::Mutex& mutex) : fTree(tree), fMutex(mutex) {
  }
  inline hist::Manager::~Manager() {
    DeleteAll();
  }
  inline void hist::Manager::DeleteAll() {
    LockingPointer<hist::Container_t> pSet(fSet, TTHREAD_GLOBAL_MUTEX);
    Int_t nhists = pSet->size();
    std::vector<rb::Hist*> addresses(pSet->begin(), pSet->end());
    for(Int_t i=0; i< nhists; ++i) delete addresses[i]; // removes from pSet
  }
  inline void hist::Manager::Add(rb::Hist* hist) {
    LockingPointer<hist::Container_t> pSet(fSet, TTHREAD_GLOBAL_MUTEX);
    pSet->insert(hist);
  }
  inline void hist::Manager::Remove(rb::Hist* hist) {
    LockingPointer<hist::Container_t> pSet(fSet, TTHREAD_GLOBAL_MUTEX);
    pSet->erase(hist);
    TDirectory* directory = hist->fDirectory;
    if(directory) directory->Remove(hist);
  }




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

