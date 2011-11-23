/*! \file main.cc
 *  \brief Implements the \c main program and Doxygen documentation.
 */
#include "TRint.h"
#include "Rootbeer.hxx"

namespace rb
{
  /// Class that runs the interactive ROOT application.
  /*! We can essentially use the normal <tt>TRint</tt>, except
   *  we need to override the Terminate()
   *  method to stop threaded processes. */
  class Rint : public TRint
  {
  public:
    /// Constructor

    /// Just call the standard \c TRint constructor, plus set the prompt to be
    /// <tt>rootbeer [\%d]</tt>.
    /// \note The \c \%d means that the number of commands entered in the session is
    /// what's present.
    Rint(const char* appClassName, int* argc, char** argv,
	 void* options = 0, int numOptions = 0, Bool_t noLogo = kFALSE) :
      TRint(appClassName, argc, argv, options, numOptions, noLogo) {
      SetPrompt("rootbeer [%d] ");
    }

    /// Terminate the application, first closing any threaded processes that might be running.
    void Terminate(Int_t status = 0) {
      rb::canvas::StopUpdate();
      rb::Unattach();
      TRint::Terminate(status);
    }

    /// Destructor

    /// Calls \c Terminate() with error code.
    ~Rint() {
      Terminate(EXIT_FAILURE);
    }
  };
}

/// The \c main ROOTBEER function.
/*! Creates an instance of \c rb::Rint and runs it. */
Int_t main(Int_t argc, Char_t** argv)
{
  rb::Rint rbApp("ROOTBEER", &argc, argv, 0, 0, kTRUE);
  rb::Logo();
  rbApp.Run();
  return 0;
}


// ----- END CODE ---- //




/*!
  \mainpage ROOTBEER: The ROOT Basic Event ExtractoR
  \authors G. Christian \n gchristian@triumf.ca \n\n



  \n \section intro Introduction

\verbatim
                                     ___
                                   .'   '.
                                  /       \           oOoOo
                                 |         |       ,==|||||
                                  \       /       _|| |||||
                                   '.___.'    _.-'^|| |||||
                                 __/_______.-'     '==HHHHH
                            _.-'` /                   """""
                         .-'     /   oOoOo
                         `-._   / ,==|||||
                             '-/._|| |||||
                              /  ^|| |||||
                             /    '==HHHHH
                            /________"""""
                            `\       `\
                              \        `\   /
                               \         `\/
                               /
                              /
                             /_____

              Welcome to ROOT BEER, the ROOT Basic Event ExtractoR
\endverbatim


  ROOTBEER is an online/offline data analysis package built around <a href="http://www.root.cern.ch"> CERN ROOT</a>.
  It is intended for analysis of low to medium-energy nuclear physics experiments, though it may be useful elsewhere.
  The basic premise behind ROOTBEER is to extend the normal ROOT package, allowing analysis of incoming online data.
  This essentially means adding the following functionality to what is already contained within ROOT:

        -# Attaching to an online stream of data and calculating a pre-compiled set of parameters.
	-# Unpacking the data into histograms. The histograms should have the ability to be dynamically created,
	gated, drawn, analyzed, and destroyed.
	-# The ability to dynamically change how data is analyzed in a simple way, i.e. changing calibration variables
	and the like. Also, it should be possible to calculate and display "compound" parameters that depend on the
	pre-compiled ones.
	-# User access to the CINT command line and all of it's normal functionality.
	-# A user-friendly GUI, allowing a short learning curve for those not familiar with ROOT.

  The ROOTBEER code is essentially a framework facilitating the display of data in histograms.  The real work of
  unpacking and analyzing the data collected in a given experiment will need to be implemented as C++ code (or C code
  compatable with C++ compilers) by users. A code skeleton has been set up that will hopefully allow this to be done
  relatively painlessly.  Depending on where you collect your data, the way in which online buffers are received
  may need to be implemented as well (currently <a href="http://www.nscl.msu.edu">NSCL/MSU</a> and
  <a href="http://www.triumf.ca">TRIUMF</a> online data are supported).

  As much as possible, we want to keep things simple and make use of what has already been implemented into ROOT.
  There's no reason to re-invent the wheel, and really all we need/want to do is to add the things listed above to
  what's already in ROOT.  Furthermore, we want to preserve the normal, offline user interface as much as possible
  so that use of ROOTBEER will feel natural to those already accustomed to ROOT and CINT.

  For more detailed information about how to use ROOTBEER and how to extend it to you own experiment,
  follow the links below.


  \n \section more Further Documentation
  - \link install Installation \endlink

  - \link user Command Line Interface \endlink

  - \link gui  Graphical User Interface (GUI) \endlink

  - \link data Customizing For Your Experiment \endlink



  \page install Installation
  \todo Write installation section.


  \page user Command Line Interface
  Users can interact with the ROOTBEER via the CINT command line.  All of the normal functionality of ROOT is still
  there, and ROOTBEER simply adds a few functions specific to the analysis of incoming data. This page outlines some
  of the more common user actions:

  - \link attach Attaching to Data \endlink
  - \link hist Working with Histograms \endlink
  - \link update Updating Canvases \endlink


  For a complete list of all of the user commands and their documentation, visit the Rootbeer.hxx Doxygen page.


  \n \section attach Attaching to Data
  In ROOTBEER, it is possible to attach to either an online data source (streaming in from somewhere) or an offline
  one (data saved in a file).  To attach to an online data source, the command is
  \code
  rb::AttachOnline();
  \endcode
  ROOTBEER will now start looking for data buffers from a source defined in the compiled code.
  When it is sent buffers, ROOTBEER will unpack them into pre-compiled data structures. The user then has access
  to the data via \link hist Histograms\endlink that he/she can define. The receiving, unpacking, and histogramming
  of online data is handled "behind the scenes" in a separate thread from the one running CINT, so the user still
  has full access to the command line when online data is coming in.

  To stop receiving online buffers, use the Unattach() function:
  \code
  rb::Unattach();
  \endcode

  ROOTBEER can also read offline data from a saved file:
  \code
  rb::AttachFile("myOfflineFileName.evt");
  \endcode

  As with online data, this is run in a separate thread.

  Note that if the user calls any of the \c Attach commands while already conneted to a data source, ROOTBEER
  will first Unattach() from the old one before connecting to the new one.


  
  \n \section hist Working with Histograms

  This section describes some of the user interface to ROOTBEER histograms.
  For more information and a complete list of member functions, see the Doxygen page Hist.hxx

  To arrange streaming data into histograms, ROOTBEER contains 1D, 2D, and 3D histogram types that inherit from
  the respective \c TH*D classes in ROOT (for non C++ experts, this means that they keep all of the functionality
  of ROOT histograms, plus add a bit more). The main addition to normal ROOT histograms is the ability of the histogram
  to fill itself, i.e. each histogram is associated with an experiment parameter(s) (or some formula depending on an
  experiment parameter(s)) and fills itself with the new parameter value every time an event come in.  The same thing
  is true for gating conditions: the histogram has a gating condition associated with it and only fills for events
  where it's true.

  To allow for safe creation and use of histograms, CINT users can only create them via the \c rb::AddHist commands
  (the constructors are hidden from CINT). The reason for doing this is CINT's feature of implicitly
  allowing duplicate variable names, i.e. one can create two \c TH1D objects referenced by the same variable:
  \code
  TH1D someHist("myHist", "My Histogram", 100, -100, 100);
  TH1D someHist("myOtherHist", "My Other Histogram", 100, -100, 100);
  \endcode
  and CINT won't complain. Instead, implicitly deletes the first histogram and makes someHist to refer to the second
  one.  This doesn't really play nice with the filling of histograms behind the scenes in another thread, as is done
  in ROOTBEER, so it's best to avoid the issue altogether. One way of doing this is to create histograms via function
  calls and never state an explicit variable name. The function can create the histogram behind the scenes, and then
  thanks to CINT's helpful feature of automatically creating pointers that mirror object names, we have direct
  access to it.

  The \c rb::AddHist functions used in ROOTBEER to create histograms look very much like the normal constructors
  for \c TH*D objects.
  The only real difference is the addition of arguments that specify the parameter with which the histogram is filled
  and it's gating condition.  For example, to create a 1D histogram displaying parameter <tt>a</tt> in 100 bins from
  -100 to 100, type:
  \code
  rb::AddHist("myHistName", "My histogram title", 100, -100, 100, "a", "");
  \endcode

  We have now added a histogram to the ROOTBEER environment. The user has access via the \c myHistName pointer
  and and use it to access all of the normal histogram functionality in ROOT,  i.e. one can do
  \code
  myHistName->Draw();
  myHistName->Fit("gaus");
  \endcode
  or whatever. Experienced ROOT users will notice that the first five arguments are exactly the same as those of
  a \c TH1D constructor. The 6th argument specifies that the histogram should fill itself with parameter <tt>a</tt>,
  and the 7th specifies that there is no gate condition applied.  Two and three-dimensional histograms are created
  similarly, with the parameter specifier still being a single string with parameters separated by colons,
  as in \c <a href = "http://root.cern.ch/root/html/TTree.html#TTree:Draw%1">TTree::Draw</a>. For example, to plot
  parameter \c b (y-axis, 100 bins from -100 to 100) vs. \c a (x-axis, 100 bins from -100 to 100),
  with the condition that <tt>b != -1</tt>:
  \code
   rb::AddHist("histab", "B vs A", 100, -100, 100, 100, -100, 100, "b:a", "b != -1");
  \endcode

  The gate argument is the same as that of \c <a href = "http://root.cern.ch/root/html/TTree.html#TTree:Draw%1">
  TTree::Draw</a>, i.e. a logical C++ statement depending on valid parameters.  As mentioned, the parameter and
  gate fields can also be compound variables, calculated from one or more static parameters. For example to plot
  <tt>sqrt(a) / 2</tt>:
  \code
  rb::AddHist("sqrta2", "Root of a divided by 2", 100, -100, 100, "sqrt(a)/2", "");
  \endcode

  In the case that an invalid parameter or gate argument is entered, the user will get an error message, and no
  new histogram will be created.  To avoid problems with duplicate naming, if the user tries to create a new
  histogram and specifies a name that is already in use (for any type of object),
  ROOTBEER appends _1, _2, etc. until the name is unique.

  Histograms can also be re-gated, using the Regate() member function. The argument is simply the new
  gate condition:
  \code
  myHist->Regate("a > 5");
  \endcode
  Now \c myHist will only fill with parameter \c a is greater than 5.

  It is also possible to zero histograms while data is coming in. For this, use the \c Clear() function:
  \code
  myHist->Clear();
  \endcode

  Filling of histograms is done behind the scenes if we're attached to a data source. However it is also
  possible to manually fill the higtogram with the current value of it's internal parameter (and subject
  to it's internal gate condition)
  \code
  myHist->Fill();
  \endcode

  Note that you can also use the normal \c <a href="http://root.cern.ch/root/html/TH1.html">TH*D::Fill</a>
  methods to fill with specific values not related to the internal histogram parameters, and without regard
  for the gate condition. For example, to fill \c myHist with 19.3
  \code
  myHist->Fill(19.3);
  \endcode


  \n \section update Updating Canvases

  Once you have drawn a histogram on a canvas, it is possible to have ROOTBEER automatically refresh it
  at a set update rate to reflect new incoming data.  This is done via a call to the rb::canvas::StartUpdate()
  function:
  \code
  rb::canvas::StartUpdate(5);
  \endcode
  This will cause ROOTBEER to update every open canvas evert five seconds. To stop auto-update, use the
  StopUpdate() function:
  \code
  rb::canvas::StopUpdate();
  \endcode

  One can also update all canvases manually
  \code
  rb::canvas::UpdateAll();
  \endcode
  or just the current one
  \code
  rb::canvas::UpdateCurrent();
  \endcode


  \page gui Graphical User Interface
  \todo Write GIU section


  \page data Data Classes
  \todo Write section on implementing user data.










*/
