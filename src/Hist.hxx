/*! \file Hist.hxx
 *  \brief Defines the histogram class.
 *  used in <tt>ROOTBEER</tt>.
 */
#ifndef __HIST__
#define __HIST__
#include <string>
#include <sstream>
#include <vector>
#include <list>
#include "TH1D.h"
#include "TH2D.h"
#include "TH3D.h"
#include "TTree.h"
#include "TTreeFormula.h"
#include <TROOT.h>
#include <TError.h>
#include <TObjArray.h>
#include <TMutex.h>
#include "LockingPointer.hxx"


namespace rb
{
  ///
  class Hist : public TNamed
  {
    typedef std::list<rb::Hist*> List_t;

  private:
    /// Number of dimensions
    UInt_t kDimensions;

    /// The original title
    //! Used for changing the title in Regate()
    std::string kInitialTitle;

    /// Default title, set from the parameters and gate condition.
    std::string kDefaultTitle;

    /// Internal histogram.
    //    volatile TH1* fHistogram;

    /// Clone of the internal histogram.
    //! This is basically the only thing that CINT users can access, via the GetHist() function.
    //! The reason for doing it this way is thread safety. By only allowing CINT users access to
    //! a copy of fHistogram (created within a mutex lock), we ensure that there will never be any
    //! conflicts between the main therad and others that can modify the internal histogram.
    TH1* fHistogramClone;

    /// Directory owning a rb::Hist instance.
    TDirectory* fDirectory;

    /// Gate formula.
    //    volatile TTreeFormula* fGate;

    /// Parameter formulae.
    //    volatile std::vector<TTreeFormula*> fParams;

    /// Internal mutex.
    TMutex fMutex;

    /// Global mutex.
    static TMutex fgMutex;

    /// Static list of all existing rb::Hist derived objects.
    static volatile List_t fgList;

    /// Static TTree for calculating parameter values.
    static volatile TTree fgTree;

    /// Constructor error code: true = success, false = failure.
    Bool_t kConstructorSuccess;

    /// Function to initialize a histogram.
    //! Basically this is just a helper function that is called by the various
    //! 1d, 2d, 3d flavors of rb::AddHist().  It handles the following:
    //!    - Creation of the rb::Hist object itself via the protected constructor.
    //!    - Initialization of the internal fHistogram TH1* pointer to the appropriate type
    //!      (e.g. TH1D for 1d, etc)
    //!    - Adding of the rb::Hist object to both ROOT's gDirectory->fList and the static
    //!      rb::Hist::fgList list of all rb::Hist objects.
    static Bool_t Initialize(const char* name, const char* title,
			     const char* param, const char* gate, UInt_t ndim,
			     Int_t nbinsx, Double_t xlow, Double_t xhigh,
			     Int_t nbinsy, Double_t ylow, Double_t yhigh,
			     Int_t nbinsz, Double_t zlow, Double_t zhigh);

    /// Constructor
    //! Set the internal \c TTree and \c fGate fields.
    //! \note Protected b/c we shouldn't create explicit instances of this.
    Hist(const char* name, const char* title, const char* param, const char* gate, UInt_t npar);

    volatile struct CriticalElements
    {
      TH1* fHistogram;
      TTreeFormula* fGate;
      std::vector<TTreeFormula*> fParams;
      TTree* GetTree() {
	return const_cast<TTree*>(&fgTree);
      }
      List_t* GetList() {
	return const_cast<List_t*>(&fgList);
      }
    } fCritical;


  public:
    /// Function to fill histogram from its internal parameter value(s).
    virtual Int_t Fill();

    /// Returns a copy of fHistogram.
    TH1* GetHist();

    /// Draw function.
    //! Calls GetHist() and then Draw() one the result.
    void Draw(Option_t* option = "") {
      LockingPointer<CriticalElements>(fCritical, fgMutex)->fHistogram->Draw(option);
    }

    /// Clear function.
    void Clear() {
      LockingPointer<CriticalElements> critical(fCritical, fgMutex);
      TH1D* hist = static_cast<TH1D*>(critical->fHistogram);
      for(Int_t p = 0; p < hist->fN; ++p) hist->fArray[p] = 0.;
    }

    /// Default constructor.
    Hist() { } ;

    /// Destructor
    /*! Free resorces allocted to TTreeFormulas */
    virtual ~Hist();

    /// Function to change the histogram gate.
    //! Updates \c fGate to reflect the new gate formula. Returns 0 if successful,
    //!  -1 if \c newgate isn't valid. In case of invalid \c newgate, the histogram
    //!  gate condition remains unchanged.
    virtual Int_t Regate(const char* newgate);

    /// Return the number of dimensions.
    UInt_t GetNdimensions() {
      return kDimensions;
    }

    /// Return the gate argument
    std::string GetGate() {
      return LockingPointer<CriticalElements>(fCritical, fgMutex)->fGate->GetExpFormula().Data();
    }

    /// Return the parameter name for the specified axis.
    std::string GetParam(UInt_t axis);

    ///
    static TMutex& GetMutex() {
      return fgMutex;
    }

    /// Function to fill all histograms
    static void FillAll();

    /// Function to access fgTree
    static TTree* GetTreeClone() {
      return static_cast<TTree*> (LockingPointer<TTree>(fgTree, fgMutex)->Clone());
    }

    /// Function to add branches to fgTree.
    static TBranch* AddBranch(const char* name, const char* classname, void** obj,
			      Int_t bufsize = 32000, Int_t splitlevel = 99) {
      return LockingPointer<TTree>(fgTree, fgMutex)->Branch(name, classname, obj, bufsize, splitlevel);
    }

    /// Function to access entries of \c Hist::fgList
    static rb::Hist* Find(const char* name) {
      LockingPointer<List_t> hlist(fgList, fgMutex);
      List_t::iterator it;
      for(it = hlist->begin(); it != hlist->end(); ++it) {
	if(!strcmp((*it)->GetName(), name)) return *it;
      }
      fprintf(stderr, "Error in <rb::Hist::Find>: %s was not found\n", name);
      return 0;
    }

    /// Function to delete all entries of \c Hist::fgList
    static void DeleteAll();

    /// Function to tell the total number of entries in \c fgList
    static UInt_t GetNumber() {
      return LockingPointer<List_t>(fgList, fgMutex)->size();
    }

    /// Set an alias in \c fgTree
    static Bool_t SetAlias(const char* aliasName, const char* aliasFormula) {
      LockingPointer<TTree>(fgTree, fgMutex)->SetAlias(aliasName, aliasFormula);
    }

    /// Static mutex locking function
    static Int_t Lock() {
      return fgMutex.Lock();
    }

    /// Static mutex try-to-lock function
    //! Returns 0 on success.
    static Int_t TryLock() {
      return fgMutex.TryLock();
    }

    /// Static mutex un-locking function
    static Int_t Unlock() {
      return fgMutex.UnLock();
    }

    /// One-dimensional creation function
    static void New(const char* name, const char* title,
		    Int_t nbinsx, Double_t xlow, Double_t xhigh,
		    const char* param, const char* gate = "");

    /// Two-dimensional creation function
    static void New(const char* name, const char* title,
		    Int_t nbinsx, Double_t xlow, Double_t xhigh,
		    Int_t nbinsy, Double_t ylow, Double_t yhigh,
		    const char* param, const char* gate = "");

    /// Three-dimensional creation function
    static void New(const char* name, const char* title,
		    Int_t nbinsx, Double_t xlow, Double_t xhigh,
		    Int_t nbinsy, Double_t ylow, Double_t yhigh,
		    Int_t nbinsz, Double_t zlow, Double_t zhigh,
		    const char* param, const char* gate = "");

    /// \c CINT Classdef.
    ClassDef(Hist, 0);
  };

}




#endif
