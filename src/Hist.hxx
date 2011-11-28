/*! \file Hist.hxx
 *  \brief Defines the histogram classes
 *  used in <tt>ROOTBEER</tt>.
 *
 *  The essential design is to inherit everything
 *  from the appropriate-dimensioned <tt>TH*F</tt>.
 *  See comments in the individual classes for details.
 *  \todo Check on thread safety of other access methods.
 */
#ifndef __HIST__
#define __HIST__
#include <string>
#include <sstream>
#include <vector>
#include "TH1D.h"
#include "TH2D.h"
#include "TH3D.h"
#include "TTree.h"
#include "TTreeFormula.h"
#include "TROOT.h"
#include "TError.h"
#include "TObjArray.h"
#include "TMutex.h"


namespace rb
{
  /// Forward declaration of rb::Hist derived classes.
  class H1D;
  class H2D;
  class H3D;

  /// Base class for \c rb::HnF objects.
  /*! Main purpose is to allow polymorphism for the \c Fill()
   *  function (n.b., that's \c Fill() with no arguments).
   *  Also implements some other functions shared by
   *  the various \c rb::HnF flavors
   *  \note To avoid multi-inheritance issues, this class should
   *  <i>not</i> define any methods
   *  that are contained in \c TH1 or it's derivatives.
   */
  class Hist
  {  
  protected:
    /// Gate formula.
    TTreeFormula* fGate;

    /// Parameter formulae.
    std::vector<TTreeFormula*> fParams;

    /// Static array of all existing rb::Hist derived objects.
    static TObjArray fgArray;

    /// Static mutex for locking threaded histogram actions.
    static TMutex fgMutex;

    /// Static TTree for calculating parameter values.
    static TTree fgTree;

    /// Constructor error code: true = success, false = failure.
    static Bool_t kConstructorSuccess;


  public:
    /// Function to fill histogram from its internal parameter value(s).
    virtual Int_t Fill() = 0;

    /// Constructor
    /*! Set the internal \c TTree and \c fGate fields. */
    Hist(const char* param, const char* gate, UInt_t npar);

    /// Default constructor.
    Hist() { } ;

    /// Destructor
    /*! Free resorces allocted to TTreeFormulas */
    virtual ~Hist();

    /// Function to change the histogram gate.
    /*! Updates \c fGate to reflect the new gate formula. Returns 0 if successful,
     *  -1 if \c newgate isn't valid. In case of invalid \c newgate, the histogram
     *  gate condition remains unchanged.
     * \bug Calling this while attached online and updating at 1 sec caused a lock-up.
     * Update: probably fixed but needs more testing, see the note about TTreeFormula::Compile().
     */
    virtual Int_t Regate(const char* newgate);

    /// Return the number of dimensions.
    UInt_t GetNdimensions() {
      return fParams.size();
    }

    /// Return the gate argument
    std::string GetGate();

    /// Return the parameter name for the specified axis.
    std::string GetParam(Int_t axis);

    /// Function to fill al hostograms
    static void FillAll();

    /// Function to access fgTree
    static TTree* GetTree() { return &fgTree; }

    /// Function to add branches to fgTree.
    static TBranch* AddBranch(const char* name, const char* classname, void** obj,
			      Int_t bufsize = 32000, Int_t splitlevel = 99);

    /// Function to access entries of \c Hist::fgArray
    static rb::Hist* Get(UInt_t index);

    /// Function to tell the total number of entries in \c fgArray
    static UInt_t GetNumber();

    /// Set an alias in \c fgTree
    static Bool_t SetAlias(const char* aliasName, const char* aliasFormula);

    /// Static mutex locking function
    static Int_t Lock();

    /// Static mutex un-locking function
    static Int_t Unlock();

    /// \brief Function to create a 1d histogram. Mirors the \c TH1D constructor.
    static void New(const char* name, const char* title,
		    Int_t nbinsx, Double_t xlow, Double_t xhigh,
		    const char* param, const char* gate = "");

    /// \brief Function to create a 2d histogram. Mirors the \c TH2D constructor.
    static void New(const char* name, const char* title,
		    Int_t nbinsx, Double_t xlow, Double_t xhigh,
		    Int_t nbinsy, Double_t ylow, Double_t yhigh,
		    const char* param, const char* gate = "");

    /// \brief Function to create a 3d histogram. Mirors the \c TH3D constructor.
    static void New(const char* name, const char* title,
		    Int_t nbinsx, Double_t xlow, Double_t xhigh,
		    Int_t nbinsy, Double_t ylow, Double_t yhigh,
		    Int_t nbinsz, Double_t zlow, Double_t zhigh,
		    const char* param, const char* gate = "");
  };


  /// \brief One-dimensional histogram class for <tt>rootbeer</tt>.
  /*! Essentially inherits all of the functionality of a normal
   *  <tt>TH1D</tt>. The main additions are the ability to fill (and gate)
   *  itself from an internal \c TTree pointer, using <tt>TTreeFormula</tt>,
   *  and thread safety. For the thread safety, we override normal \c TH1D
   *  methods to call the normal functionality within mutex locks.  For this,
   *  we make use of the static \c rb::Hist::Lock() and \c rb::Hist::Unlock() functions.
   */
  class H1D : public TH1D, public Hist
  {

  public:
    /// Constructor
    /*! Format basically mirrors the normal TH1D constructor but
      with the addition of \c param and \c gate arguments
      to set the \c fParam and \c fGate fields. Also adds to the
      static Hist::fgArray list.
      Locks the \c gUnpacker mutex when adding to fgArray
      \note The constructor is hidden from \c CINT because we
      don't want users to be able to call it. Instead they use the
      rb::AddHist() function. The reason for this choice is that
      CINT allows users to duplicate symbols, deleting the old
      object, and this doesn't play nice with the threaded filling
      of all histograms in Hist::fgArray. Giving access via functions
      eliminates the problem and still allows users simple access via
      the atomatic pointers that CINT creates.
    */
#ifndef __CINT__
    H1D (const char* name, const char* title,
         Int_t nbins, Double_t xlow, Double_t xhigh,
	 const char* param, const char* gate = "");
#endif

    /// Empty constructor for \c CINT
    H1D() { };

    /// Destructor.
    /*! Remove from Hist::fgAarray.*/
    ~H1D();

    /// Draw function
    /*! Just calls the normal \c TH1D::Draw but with the 
      mutex locked. */
    void Draw(Option_t* option);

    /// Fill function
    /*! Evaluates \c fParam and calls \c TH1D::Fill using the result.
      Also locks the \c gUnpacker mutex for thread safety. */
    Int_t Fill();

    /// Allows the user to zero out the histogram.
    void Clear(const Option_t* option = "");

    /// ClassDef for <tt>CINT</tt>.
    ClassDef(rb::H1D, 0);
  };


  /// \brief Two-dimensional histogram class for <tt>rootbeer</tt>.
  /*! Essentially inherits all of the functionality of a normal
   *  <tt>TH2D</tt>. The main additions are the ability to fill (and gate)
   *  itself from an internal \c TTree pointer, using <tt>TTreeFormula</tt>,
   *  and thread safety. For the thread safety, we override normal \c TH2D
   *  methods to call the normal functionality within mutex locks.  For this,
   *  we make use of the static \c rb::Hist::Lock() and \c rb::Hist::Unlock() functions.
   */
  class H2D : public TH2D, public Hist
  {

  public:
    /// Constructor
    /*! Format basically mirrors the normal TH2D constructor but
      with the addition of \c param and \c gate arguments
      to set the \c fParam and \c fGate fields. Also adds to the
      static Hist::fgArray list.
      Locks the \c gUnpack mutex when adding to fgArray
      \note The constructor is hidden from \c CINT because we
      don't want users to be able to call it. Instead they use the
      rb::AddHist() function. The reason for this choice is that
      CINT allows users to duplicate symbols, deleting the old
      object, and this doesn't play nice with the threaded filling
      of all histograms in Hist::fgArray. Giving access via functions
      eliminates the problem and still allows users simple access via
      the atomatic pointers that CINT creates.
    */
#ifndef __CINT__
    H2D (const char* name, const char* title,
         Int_t nbinsx, Double_t xlow, Double_t xhigh,
	 Int_t nbinsy, Double_t ylow, Double_t yhigh,
	 const char* param, const char* gate = "");
#endif

    /// Empty constructor for \c CINT
    H2D() { };

    /// Destructor.
    /*! Remove from Hist::fgAarray.*/
    ~H2D();

    /// Draw function
    /*! Just calls the normal \c TH2D::Draw but with the 
      \c gUnpacker mutex locked. */
    void Draw(Option_t* option);

    /// Fill function
    /*! Evaluates \c fParam and calls \c TH2D::Fill using the result.
      Also locks the \c gUnpacker mutex for thread safety. */
    Int_t Fill();

    /// Allows the user to zero out the histogram.
    void Clear(const Option_t* option = "");

    /// ClassDef for <tt>CINT</tt>.
    ClassDef(rb::H2D, 0);
  };


  /// \brief Three-dimensional histogram class for <tt>rootbeer</tt>.
  /*! Essentially inherits all of the functionality of a normal
   *  <tt>TH3D</tt>. The main additions are the ability to fill (and gate)
   *  itself from an internal \c TTree pointer, using <tt>TTreeFormula</tt>,
   *  and thread safety. For the thread safety, we override normal \c TH3D
   *  methods to call the normal functionality within mutex locks.  For this,
   *  we make use of the static \c rb::Hist::Lock() and \c rb::Hist::Unlock() functions.
   */
  class H3D : public TH3D, public Hist
  {

  public:
    /// Constructor
    /*! Format basically mirrors the normal TH3D constructor but
      with the addition of \c param and \c gate arguments
      to set the \c fParam and \c fGate fields. Also adds to the
      static Hist::fgArray list.
      Locks the \c gUnpacker mutex when adding to fgArray
      \note The constructor is hidden from \c CINT because we
      don't want users to be able to call it. Instead they use the
      rb::AddHist() function. The reason for this choice is that
      CINT allows users to duplicate symbols, deleting the old
      object, and this doesn't play nice with the threaded filling
      of all histograms in Hist::fgArray. Giving access via functions
      eliminates the problem and still allows users simple access via
      the atomatic pointers that CINT creates.
    */
#ifndef __CINT__
    H3D (const char* name, const char* title,
         Int_t nbinsx, Double_t xlow, Double_t xhigh,
	 Int_t nbinsy, Double_t ylow, Double_t yhigh,
	 Int_t nbinsz, Double_t zlow, Double_t zhigh,
	 const char* param, const char* gate = "");
#endif

    /// Empty constructor for \c CINT
    H3D() { };

    /// Destructor.
    /*! Remove from Hist::fgAarray.*/
    ~H3D();

    /// Draw function
    /*! Just calls the normal \c TH3D::Draw but with the 
      \c gUnpack mutex locked. */
    void Draw(Option_t* option);

    /// Fill function
    /*! Evaluates \c fParam and calls \c TH3D::Fill using the result.
      Also locks the \c gUnpacker mutex for thread safety. */
    Int_t Fill();

    /// Allows the user to zero out the histogram.
    void Clear(const Option_t* option = "");

    /// ClassDef for <tt>CINT</tt>.
    ClassDef(rb::H3D, 0);
  };


}

#endif
