/*! \file Hist.hxx
 *  \brief Defines the histogram classes
 *  used in <tt>ROOTBEER</tt>.
 *
 *  The essential design is to inherit everything
 *  from the appropriate-dimensioned <tt>TH*F</tt>.
 *  See comments in the individual classes for details.
 *  \todo Automatic TCutG variables, Getters for parameter values.
 */
#ifndef __HIST__
#define __HIST__
#include <string>
#include <sstream>
#include "TH1D.h"
#include "TH2D.h"
#include "TH3D.h"
#include "TTreeFormula.h"
#include "TROOT.h"
#include "TError.h"

#include "Rootbeer.hxx"


namespace rb
{

  /// Base class for \c rb::HnF objects.
  /*! Main purpose is to allow polymorphism for the \c Fill()
   *  function (n.b., that's \c Fill() with no arguments).
   *  Also implements some helper functions shared by
   *  the various \c rb::HnF flavors.
   *  \note To avoid multi-inheritance issues, this function should
   *  <i>not</i> implement any methods
   *  that are contained in \c TH1 or it's derivatives. */
  class Hist
  {
  protected:
    /// Function to turn an empty gate argument into
    /// one that is always true.
    static std::string CheckGate(const char* gate) {
      if(std::string(gate) == "") return "1" ;
      else return gate ;
    }

    /// Function to append \c _n to duplicate names.
    static std::string CheckName(const char* name) {
      std::string ret = name;
      if(gROOT->FindObject(name)) {
	Int_t n = 1;
	while(1) {
	  std::stringstream sstr;
	  sstr << name << "_" << n++;
	  ret = sstr.str();
	  if(!gROOT->FindObject(ret.c_str())) break;
	}
	Info("AddHist", "The name %s is already in use, creating %s_%d instead.", name, name, n-1);
      }
      return ret;
    }

    /// Function to parse 2d parameter arguments into variables.
    static std::string ParseParam2d(const char* par, Int_t axis) {
      std::string spar (par);
      Int_t p1 = spar.find(":");
      std::string ret = "";
      switch(axis) {
      case 1: ret = spar.substr(0, p1); break;
      case 0: ret = spar.substr(p1+1); break;
      default:
	Error("ParseParam2d",
	      "Invalid axis specification %d (should be x:0, y:1)",
	      axis);
	break;
      }
      return ret;
    }

    /// Function to parse 3d parameter arguments into variables.
    static std::string ParseParam3d(const char* par, Int_t axis) {
      std::string spar (par);
      Int_t p1 = spar.find(":");
      Int_t p2 = spar.find_last_of(":");
      std::string ret = "";
      switch(axis) {
      case 2: ret = spar.substr(0, p1);         break;
      case 1: ret = spar.substr(p1+1, p2-p1-1); break;
      case 0: ret = spar.substr(p2+1);          break;
      default:
	Error("ParseParam3d",
	      "Invalid axis specification %d (x = 0, y = 1, z = 2)",
	      axis);
	break;
      }
      return ret;
    }

  public:
    /// Function to fill histogram from its internal \c fTree value(s).
    virtual Int_t Fill() = 0;

    /// Destructor
    virtual ~Hist() { };
  };


  /// \brief One-dimensional histogram class for <tt>rootbeer</tt>.
  /*! Essentially inherits all of the functionality of a normal
   *  <tt>TH1D</tt>. The main additions are the ability to fill (and gate)
   *  itself from an internal \c TTree pointer, using <tt>TTreeFormula</tt>,
   *  and thread safety. For the thread safety, we override normal \c TH1D
   *  methods to call the normal functionality within mutex locks.  For this,
   *  we make use of the static \c HistMutex::Lock() and \c HistMutex::Unlock() functions.
   *  \todo A lot more of the access methods need to be made thread safe!!!
   *  \todo Copy constructor.
   */
  class H1D : public TH1D, public Hist
  {

  protected:
    /// Internal TTree for dynamic filling.
    TTree* fTree;

    /// Parameters evaluated from <tt>fTree</tt>.
    TTreeFormula fParam;

    /// Gate evaluated from <tt>fTree</tt>.
    TTreeFormula fGate;

  public:
    /// Constructor
    /*! Format basically mirrors the normal TH1D constructor but
      with the addition of \c param and \c gate arguments
      to set the \c fParam and \c fGate fields. Also adds to the
      global \c gHistograms list.
      Locks the \c gUnpacker mutex when adding to
      <tt>ghistograms</tt>.
      \note The constructor is hidden from \c CINT because we
      don't want users to be able to call it. Instead they use the
      \c rb::AddHist function. The reason for this choice is that
      \c CINT allows users to duplicate symbols, deleting the old
      object, and this doesn't play nice with the \c gHistograms
      list used in <tt>ROOTBEER</tt>. Giving access via functions
      eliminates the problem and still allows users simple access via
      the atomatic pointers that \c CINT creates.
    */
#ifndef __CINT__
    H1D (const char* name, const char* title,
         Int_t nbins, Double_t xlow, Double_t xhigh,
	 const char* param, const char* gate = "");
#endif

    /// Empty constructor for \c CINT
    H1D() { };

    /// Destructor.
    /*! Remove from <tt>gHistograms</tt>.*/
    ~H1D();

    /// Draw function
    /*! Just calls the normal \c TH1D::Draw but with the 
        mutex locked. */
    void Draw(Option_t* option);

    /// Fill function
    /*! Evaluates \c fParam and calls \c TH1D::Fill using the result.
      Also locks the \c gUnpacker mutex for thread safety. */
    Int_t Fill();

    /// Change the gating condition to be something else.
    /*! Updates \c fGate to reflect the new gate formula. Returns 0 if successful,
     *  -1 if \c newgate isn't valid. In case of invalid \c newgate, the histogram
     *  gate condition remains unchanged. */
    Int_t Regate(const char* newgate);

    /// Allows the user to zero out the histogram.
    void Clear(Option_t* option = "");

    /// Allow rb::AddHist access to protected class members.
    friend void AddHist(const char* name, const char* title,
			Int_t nbinsx, Double_t xlow, Double_t xhigh,
			const char* param, const char* gate);

    /// ClassDef for <tt>CINT</tt>.
    ClassDef(rb::H1D, 0);
  };


  /// \brief Two-dimensional histogram class for <tt>rootbeer</tt>.
  /*! Essentially inherits all of the functionality of a normal
   *  <tt>TH2D</tt>. The main additions are the ability to fill (and gate)
   *  itself from an internal \c TTree pointer, using <tt>TTreeFormula</tt>,
   *  and thread safety. For the thread safety, we override normal \c TH2D
   *  methods to call the normal functionality within mutex locks.  For this,
   *  we make use of the static \c HistMutex::Lock() and \c HistMutex::Unlock() functions.
   *  \todo A lot more of the access methods need to be made thread safe!!!
   *  \todo Copy constructor.
   */
  class H2D : public TH2D, public Hist
  {

  protected:
    /// Internal TTree for dynamic filling.
    TTree* fTree;

    /// Parameters evaluated from <tt>fTree</tt>.
    TTreeFormula fParamX, fParamY;

    /// Gate evaluated from <tt>fTree</tt>.
    TTreeFormula fGate;


  public:
    /// Constructor
    /*! Format basically mirrors the normal TH2D constructor but
      with the addition of \c param and \c gate arguments
      to set the \c fParam and \c fGate fields. Also adds to the
      global \c gHistograms list.
      Locks the \c gUnpacker mutex when adding to
      <tt>ghistograms</tt>.
      \note The constructor is hidden from \c CINT because we
      don't want users to be able to call it. Instead they use the
      \c rb::AddHist function. The reason for this choice is that
      \c CINT allows users to duplicate symbols, deleting the old
      object, and this doesn't play nice with the \c gHistograms
      list used in <tt>ROOTBEER</tt>. Giving access via functions
      eliminates the problem and still allows users simple access via
      the atomatic pointers that \c CINT creates.
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
    /*! Remove from <tt>gHistograms</tt>.*/
    ~H2D();

    /// Draw function
    /*! Just calls the normal \c TH2D::Draw but with the 
      \c gUnpacker mutex locked. */
    void Draw(Option_t* option);

    /// Fill function
    /*! Evaluates \c fParam and calls \c TH2D::Fill using the result.
      Also locks the \c gUnpacker mutex for thread safety. */
    Int_t Fill();

    /// Change the gating condition to be something else.
    /*! Updates \c fGate to reflect the new gate formula. Returns 0 if successful,
     *  -1 if \c newgate isn't valid. In case of invalid \c newgate, the histogram
     *  gate condition remains unchanged. */
    Int_t Regate(const char* newgate);

    /// Allows the user to zero out the histogram.
    void Clear(Option_t* option = "");

    /// Allow rb::AddHist access to protected class members.
    friend void AddHist(const char* name, const char* title,
			Int_t nbinsx, Double_t xlow, Double_t xhigh,
			Int_t nbinsy, Double_t ylow, Double_t yhigh,
			const char* param, const char* gate);

    /// ClassDef for <tt>CINT</tt>.
    ClassDef(rb::H2D, 0);
  };


  /// \brief Three-dimensional histogram class for <tt>rootbeer</tt>.
  /*! Essentially inherits all of the functionality of a normal
   *  <tt>TH3D</tt>. The main additions are the ability to fill (and gate)
   *  itself from an internal \c TTree pointer, using <tt>TTreeFormula</tt>,
   *  and thread safety. For the thread safety, we override normal \c TH3D
   *  methods to call the normal functionality within mutex locks.  For this,
   *  we make use of the static \c HistMutex::Lock() and \c HistMutex::Unlock() functions.
   *  \todo A lot more of the access methods need to be made thread safe!!!
   *  \todo Copy constructor.
   */
  class H3D : public TH3D, public Hist
  {

  protected:
    /// Internal TTree for dynamic filling.
    TTree* fTree;

    /// Parameters evaluated from <tt>fTree</tt>.
    TTreeFormula fParamX, fParamY, fParamZ;

    /// Gate evaluated from <tt>fTree</tt>.
    TTreeFormula fGate;


  public:
    /// Constructor
    /*! Format basically mirrors the normal TH3D constructor but
      with the addition of \c param and \c gate arguments
      to set the \c fParam and \c fGate fields. Also adds to the
      global \c gHistograms list.
      Locks the \c gUnpacker mutex when adding to
      <tt>ghistograms</tt>.
      \note The constructor is hidden from \c CINT because we
      don't want users to be able to call it. Instead they use the
      \c rb::AddHist function. The reason for this choice is that
      \c CINT allows users to duplicate symbols, deleting the old
      object, and this doesn't play nice with the \c gHistograms
      list used in <tt>ROOTBEER</tt>. Giving access via functions
      eliminates the problem and still allows users simple access via
      the atomatic pointers that \c CINT creates.
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
    /*! Remove from <tt>gHistograms</tt>.*/
    ~H3D();

    /// Draw function
    /*! Just calls the normal \c TH3D::Draw but with the 
      \c gUnpacker mutex locked. */
    void Draw(Option_t* option);

    /// Fill function
    /*! Evaluates \c fParam and calls \c TH3D::Fill using the result.
      Also locks the \c gUnpacker mutex for thread safety. */
    Int_t Fill();

    /// Change the gating condition to be something else.
    /*! Updates \c fGate to reflect the new gate formula. Returns 0 if successful,
     *  -1 if \c newgate isn't valid. In case of invalid \c newgate, the histogram
     *  gate condition remains unchanged. */
    Int_t Regate(const char* newgate);

    /// Allows the user to zero out the histogram.
    void Clear(Option_t* option = "");

    /// Allow rb::AddHist access to protected class members.
    friend void AddHist(const char* name, const char* title,
			Int_t nbinsx, Double_t xlow, Double_t xhigh,
			Int_t nbinsy, Double_t ylow, Double_t yhigh,
			Int_t nbinsz, Double_t zlow, Double_t zhigh,
			const char* param, const char* gate);

    /// ClassDef for <tt>CINT</tt>.
    ClassDef(rb::H3D, 0);
  };


}

#endif
