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
#include <TF1.h>
#include <TFitResultPtr.h>
#include <TVirtualHistPainter.h>
#include "Formula.hxx"
#include "hist/Visitor.hxx"
#include "hist/Manager.hxx"
#include "utils/Error.hxx"
#include "utils/LockingPointer.hxx"
#include "utils/Critical.hxx"
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
	if(!fMutex) rb::TThreadMutex::Instance()->Lock();
	else        fMutex->Lock();
      }
      void Unlock() {
	if(!fMutex) rb::TThreadMutex::Instance()->UnLock();
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
    protected:
      /// Locks a mutex before be get to TH1D creation in the constructor
      hist::LockOnConstruction fLockOnConstruction;

      /// Stops addition of local histogram to gDirectory
      hist::StopAddDirectory fStopAdd;

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

      /// Initial parameter argumens
      std::string kInitialParams;

      /// Use the default title as the actual title?
      Bool_t kUseDefaultTitle;

      /// Directory owning this rb::hist::Base instance.
      TDirectory* fDirectory;

      /// Wrapper for the TTreeFormulae to evaluate the parameter values.
      boost::scoped_ptr<rb::TreeFormulae> fParams;

      /// Wrapper for the TTreeFormulae to evaluate the gate condition
      boost::scoped_ptr<rb::TreeFormulae> fGate;

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
      //! \Warning users should \em not delete the returned histogram. Internally, the class
      //! maintains only a single copy; successive calls to GetHist() delete the copy currently
      //! in memory and creates a new one.
      TH1* GetHist();

      /// Clear function, zeros-out all axes of the internal histogram
      virtual void Clear() { visit::hist::Clear::Do(fHistVariant); }

      /// Return the number of dimensions.
      UInt_t GetNdimensions() { return kDimensions; }

      /// Return the gate argument
      virtual std::string GetGate() { return fGate->Get(0); }

      /// Return the parameter name associated with the specified axis.
      virtual std::string GetParam(Int_t axis) { return fParams->Get(axis); }

    protected:
      /// Set name and title
      void Init(const char* name, const char* title, const char* param, const char* gate, Int_t event_code);

      /// Set paramater formulae
      virtual void InitParams(const char* param, Int_t event_code);

      /// Set gate formula
      virtual void InitGate(const char* gate, Int_t event_code);

    private:
      /// Prevent assigmnent
      Base& operator= (const Base& other) { return *this; }
      /// Prevent copying
      Base(const Base& other) : kDimensions(other.kDimensions), fManager(other.fManager) {}
      /// Internal function to fill the histogram.
      //! Called from the public Fill() and FillAll(), does not do any mutex locking,
      //! instead relies on being passed already locked components.
      virtual Int_t DoFill(const std::vector<Double_t>& params);
    public:
#include "WrapTH1.hxx"
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
	Base(name, title, param, gate, manager, event_code, nbinsx, xlow, xhigh)
      {
	Init(name, title, param, gate, event_code);
	fLockOnConstruction.Unlock();
      }
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
	Base(name, title, param, gate, manager, event_code, nbinsx, xlow, xhigh, nbinsy, ylow, yhigh)
      {
	Init(name, title, param, gate, event_code);
	fLockOnConstruction.Unlock();
      }
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
	Base(name, title, param, gate, manager, event_code, nbinsx, xlow, xhigh,
	     nbinsy, ylow, yhigh, nbinsz, zlow, zhigh)
      {
	Init(name, title, param, gate, event_code);
	fLockOnConstruction.Unlock();
      }
      ClassDef(rb::hist::D3, 0);
    };
    /// \brief Summary histogram
    //! \details Histogram displaying multiple parameters at once.
    class Summary: public Base
    {
    private:
      //! Orientation codes
      enum { VERTICAL, HORIZONTAL };
      //! Orientation of the histogram
      Int_t kOrientation;
      //! Number of bins along the parameter axis
      Int_t fBins;
      //! Low end of the parameter axis
      Double_t fLow;
      //! High end of the parameter axis
      Double_t fHigh;
    public:
      //! Calls base 2d constructor with junk bin arguments, sets constants
      Summary (const char* name, const char* title, const char* param, const char* gate,
	       hist::Manager* manager, Int_t event_code,
	       Int_t nbins, Double_t low, Double_t high, Option_t* orientation);
      //! Override hist::Base parameter initialization
      virtual void InitParams(const char* params, Int_t event_code);
      //! Override hist::Base filling procedure
      virtual Int_t DoFill(const std::vector<Double_t>& params);
    private:
      //! Set the orientation (horizontal or vertical)
      void SetOrientation(Option_t* orientation);
      ClassDef(rb::hist::Summary, 0);
    };
    /// \brief Gamma histogram
    //! \details Histogram summarizing the values of multiple parameters.
    class Gamma: public Base
    {
    private:
      //! "Stops" between different axis parameters
      std::vector<Int_t> fStops;
    public:
      /// Constructor (1d)
      Gamma (const char* name, const char* title, const char* param, const char* gate,
	     hist::Manager* manager, Int_t event_code,
	     Int_t nbins, Double_t low, Double_t high);
      /// Constructor (2d)
      Gamma (const char* name, const char* title, const char* param, const char* gate,
	     hist::Manager* manager, Int_t event_code,
	     Int_t nbinsx, Double_t xlow, Double_t xhigh,
	     Int_t nbinsy, Double_t ylow, Double_t yhigh);
      /// Constructor (3d)
      Gamma (const char* name, const char* title, const char* param, const char* gate,
	     hist::Manager* manager, Int_t event_code,
	     Int_t nbinsx, Double_t xlow, Double_t xhigh,
	     Int_t nbinsy, Double_t ylow, Double_t yhigh,
	     Int_t nbinsz, Double_t zlow, Double_t zhigh);
      //! Override hist::Base parameter initialization
      virtual void InitParams(const char* params, Int_t event_code);
      //! Override hist::Base filling procedure
      virtual Int_t DoFill(const std::vector<Double_t>& params);
      ClassDef(rb::hist::Gamma, 0);
    };

    /// \brief Bitmask histogram class.

    //! \details A bitmask histogram displays the true bits in a parameter.  For each event,
    //! the bin corresponding to a given bit in a word increments if that bit is set to 1.
    class Bit: public Base
    {
    private:
      /// The number of bits displayed
      const Int_t kNumBits;
    public:
      /// Constructor (1d)
      Bit (const char* name, const char* title, const char* param, const char* gate,
	   hist::Manager* manager, Int_t event_code,
	   Int_t n_bits, Double_t ignored1 = 0, Double_t ignored2 = 1);
      //! Override hist::Base parameter initialization
      virtual void InitParams(const char* params, Int_t event_code);
      //! Override hist::Base filling procedure
      virtual Int_t DoFill(const std::vector<Double_t>& params);
      ClassDef(rb::hist::Bit, 0);
    };
  } // namespace hist
} // namespace rb


#endif

