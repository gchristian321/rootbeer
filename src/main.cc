//! \file main.cc
//! \brief Implements the \c main program and Doxygen documentation.
#include "TRint.h"
#include "Rootbeer.hxx"
#include "Hist.hxx"
#include "Data.hxx"


/// The \c main ROOTBEER function.
/*! Creates an instance of \c rb::Rint and runs it. */
Int_t main(Int_t argc, Char_t** argv)
{
  //  rb::Logo();
  rb::Rint rbApp("RootBeer", &argc, argv, 0, 0, kTRUE);
  Midas m;
  gROOT->ProcessLine("gStyle->SetOptTitle(kTRUE)");
  rbApp.Run();
  return 0;
}






// ----- END CODE ---- //




/*!
  \mainpage ROOTBEER: The ROOT Basic Event ExtractoR
  \authors G. Christian \n gchristian@triumf.ca \n\n
  \authors S. Mosby \n mosby@nscl.msu.edu \n\n
  \authors J.K. Smith \n smithj@nscl.msu.edu \n\n



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


  ROOTBEER is an online/offline data analysis package built around
  <a href="http://root.cern.ch" target="_blank"> CERN ROOT</a>.
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
  may need to be implemented as well (currently <a href="http://www.nscl.msu.edu" target="_blank">NSCL/MSU</a> and
  <a href="http://www.triumf.ca" target="_blank">TRIUMF</a> online data are supported).

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

  - \link develop For Developers \endlink



  \page install Installation

  \n \section op Operating Systems & Prerequisites
  Currently, ROOTBEER has only been tested on Unix-based systems (Linux and Apple OS/X). This doesn't mean it won't
  work on Windows too, but we've never tried (the main source of problems would likely be in the threading parts,
  though the TThread library we use is supposed to be platform-indpendent).  In any case, the present
  instructions all assume a *NIX system.

  You need to have ROOT installed on your system and have access to the standard class headers and libraries.
  Instructions on installing ROOT can be found at http://root.cern.ch/drupal/content/downloading-root  You will
  also need a C++ compiler installed, for example <a href="http://gcc.gnu.org" target="_blank">g++</a>, and some flavor of
  <a href = "http://www.gnu.org/s/make/" target="_blank">make</a>.  Otherwise, you should be
  good to go as ROOTBEER doesn't make use of any external libraries other than the ROOT ones.
  

  \n \section compile Downloading & Compiling
  The ROOTBEER source code can be downloaded from http:://trshare/triumf.ca/~gchristian/rootbeer/download/ Both .zip
  and .tar.gz formats are available.  Potential developers or those who want to stay on top of bug fixes and other
  improvements can check out the sources using <a href="http://git-scm.com" target="_blank">git</a>
  \code
  git checkout somewhere....
  \endcode
  For more information on contributing to ROOTBEER, see the \link develop Developers \endlink page.

  To compile, unpack the sources and \c cd to the top-level ROOTBEER directory.  If the location of the ROOT
  header files is not normally searched by your compiler, then you will need to edit the Makefile. Open up
  \c Makefile and follow the instructions above the <tt>\$(INCFLAGS)</tt> line.  Having done this, exit and type <tt>make</tt>.
  This should compile the \c rootbeer executable in the top-level directory as well as a shared library in the
  \c /lib directory.  If you are adding your own analysis codes to the ROOTBEER framework, then you will need
  to make some modifiations to the Makefile. These modifications are explained in the
  \link data Customizing for Your Experiment \endlink section.

  

  \page user Command Line Interface
  Users can interact with the ROOTBEER via the CINT command line.  All of the normal functionality of ROOT is still
  there, and ROOTBEER simply adds a few functions specific to the analysis of incoming data. This page outlines some
  of the more common user actions:

  - <a href=#attach><b>Attaching to Data</b></a>
  - <a href=#hist><b>Working with Histograms</b></a>
  - <a href=#canvas><b>Canvases</b></a>


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

  As with online data, this is run in a separate thread. Optionally, you can specify a second argument (a boolean)
  that tells whether to Unattach() at the end of the file (argument <tt>true</tt>, the default) or to stay attached
  and wait for more data to come in (argument <tt>false</tt>).

  It's also possible to string multiple offline files together, using the \c rb::AttachList() command. The argument
  is a string specifying the name of a file that lists the paths of the files you want to read in order.

  Note that if the user calls any of the \c Attach commands while already conneted to a data source, ROOTBEER
  will first Unattach() from the old one before connecting to the new one.


  \n \section hist Working with Histograms

  This section describes some of the user interface to ROOTBEER histograms.
  For more information and a complete list of member functions, see the Doxygen page Hist.hxx

  To arrange streaming data into histograms, ROOTBEER defines it own histogram type that wraps the stock
  ROOT ones.  The main addition to normal ROOT histograms is the ability of a ROOTBEER histogram
  to fill itself, i.e. each histogram is associated with an experiment parameter(s) (or some formula depending on an
  experiment parameter(s)) and fills the wrapped stock histogram  with the new parameter value every time an event
  comes in.  The same thing is true for gating conditions: the histogram has a gating condition associated with it
  and only fills for events where it's true.

  To allow for safe creation and use of histograms, CINT users can only create them via the \c rb::Hist::New() functions
  (the constructors are private). The reason for doing this is CINT's feature of implicitly
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

  The \c rb::Hist::New() functions used in ROOTBEER to create histograms look very much like the normal constructors
  for \c TH*D objects.
  The only real difference is the addition of arguments that specify the parameter with which the histogram is filled
  and it's gating condition.  For example, to create a 1D histogram displaying parameter <tt>a</tt> in 100 bins from
  -100 to 100, type:
  \code
  rb::Hist::New("myHistName", "My histogram title", 100, -100, 100, "a", "");
  \endcode

  We have now added a histogram to the ROOTBEER environment, and the user has access via the \c myHistName pointer.
  Because of issues with thread safety, most of the stock histogram functions are not transferred to the ROOTBEER
  histograms. Instead, the user can obtain a copy of the internal histogram using the GetHist() function:
  \code
  TH1D* myHistInternal = myHistName->GetHist();
  myHistInternal->Fit();
  myHistInternal->Draw();
  ...
  \endcode

  Note that since GetHist() returns a copy of the internal histogram, it will not continue to update as new data
  comes in. Since you will probably want to watch the histogram update as data comes in, it is possible to
  Draw() a ROOTBEER histgram directly:
  \code
  myHistName->Draw();
  \endcode
  which will display the most up-to-date version of the histogram in the canvas. There are a few other functions
  available that will be explained later.

  Two and three-dimensional histograms are created
  similarly to 1D ones, with the parameter specifier still being a single string with parameters separated by colons,
  as in <a href = "http://root.cern.ch/root/html/TTree.html#TTree:Draw%1" target="_blank">TTree::Draw</a>.
  For example, to plot parameter \c b (y-axis, 200 bins from -200 to 200) vs. \c a (x-axis, 100 bins from -100 to 100),
  with the condition that <tt>b != -1</tt>:
  \code
   rb::Hist::New("histab", "B vs A", 100, -100, 100, 200, -200, 200, "b:a", "b != -1");
  \endcode

  The gate argument is the same as that of
  <a href = "http://root.cern.ch/root/html/TTree.html#TTree:Draw%1" target="_blank">
  TTree::Draw</a>, i.e. a logical C++ statement depending on valid parameters.  As mentioned, the parameter and
  gate fields can also be compound variables, calculated from one or more static parameters. For example to plot
  <tt>sqrt(a) / 2</tt>:
  \code
  rb::Hist::New("sqrta2", "Root of a divided by 2", 100, -100, 100, "sqrt(a)/2", "");
  \endcode

  In the case that an invalid parameter or gate argument is entered, the user will get an error message, and no
  new histogram will be created.  To avoid problems with duplicate naming, if the user tries to create a new
  histogram and specifies a name that is already in use (for any type of object),
  ROOTBEER appends _1, _2, etc. until the name is unique.

  Histograms can be re-gated, using the Regate() member function. The argument is simply the new
  gate condition:
  \code
  myHist->Regate("a > 5");
  \endcode
  Now \c myHist will only fill with parameter \c a is greater than 5. To set two-dimensional coutour gates,
  you can use a normal ROOT \c <a href = "http://root.cern.ch/root/html/TCutG.html" target="_blank">TCutG</a>
  and reference it via its name in the gate field of a rb::Hist.  As with
  \c <a href = "http://root.cern.ch/root/html/TTree.html#TTree:Draw%1" target="_blank"> TTree::Draw</a>,
  you can set alises for compound histogram parameters via the static \c rb::Hist::SetAlias() function:
  \code
  rb::Hist::SetAlias("a_times_b", "a * b");
  rb::Hist::New("hst_ab", "a times b", 100, -100, 100, "a_times_b");
  \endcode

  It is also possible to zero histograms while data is coming in. For this, use the \c Clear() function:
  \code
  myHist->Clear();
  \endcode
  Note that the zeroing of histograms can also be performed via the <a href=#canvas><b>Canvas</b></a>
  on which is resides.


  \n \section canvas Canvases

  In general, you can work with <a href="http:://root.cern.ch/root/html/TCanvas.html" target="_blank">TCanvas</a>
  objects just as you would in normal ROOT.  However, ROOTBEER adds a bit of functionality relevant to the display
  of streaming data. The main addition is the ability to have ROOTBEER auto-refresh your canvases on a regular basis. 
  To Start/Stop auto-updating use the StartUpdate() or StopUpdate() functions:

  \code
  rb::Canvas::StartUpdate(5); // All canvases will now refresh every five seconds.

  rb::Canvas::StopUpdate(); // Canvases will no longer auto-refresh
  \endcode


  You can also update canvases manually, either individual or all at once
  \code
  rb::Canvas::UpdateAll(); // Update all canvases

  rb::Canvas::UpdateCurrent(); // Update just the currently selected canvas
  \endcode

  as well as clear them

  \code
  rb::Canvas::ClearAll(); // Clear all canvases

  rb::Canvas::UpdateCurrent(); // Clear just the currently selected canvas
  \endcode




  \page gui Graphical User Interface
  \todo Write GIU section


  \page data Customizing for Your Experiment

  - <a href=#data_intro><b>Introduction</b></a>
  - <a href=#data_skel><b>Filling in the Skeleton</b></a>
  - <a href=#data_other><b>Other Odds and Ends</b></a>


  \section data_intro Introduction

  ROOTBEER is intended to be a framework into which you can plug the analysis code for your experiment.
  In other words, you write the code to unpack and analyze the data from your detectors and ROOTBEER handles
  the display side of things, putting your data into histograms and so on.  As such, we've tried to set up a
  framework that is very general and puts few constraints on how your analysis code must be written. As things
  stand there are a few limits on how your analysis code must be structured in order for it to be compatible with
  ROOTBEER:

  -# It must be in C++ or "forward compatable" C (that will compile with a C++ compiler).
  -# You must encapsulate all of your data within a class or struct. Something like
  \code
  class MyData {
     Double_t a, b, c;
     ...
  };
  \endcode
  or
  \code
  typedef struct {
     Double_t a, b, c;
     ...
  } MyData;
  \endcode
  will work fine, but
  \code
  Double_t a;
  Double_t b;
  Double_t c;
  ...
  \endcode
  won't.
  -# It is not possible to include both parameters (things you calculate event-by-event) and variables (things
   you use in the calculation of parameters) within the same class, unless you are willing to put up with some
   nuisances. For example,
   \code
   class ParamClass {
      Double_t par1;
      ...
   };

   class VariableClass {
      Double_t var1;
      ...
   };
   \endcode
   works well, but 
   \code
   class ParamClass {
      Double_t par1;
      ...
   };

   class VariableClass {
      Double_t var1;
      ...
   };

   class MasterClass {
      ParamClass parameters;
      VariableClass variables;
      ...
   };
   \endcode
   will likely cause you some hassles.

   Note that the whole reason behind organizing your data into classes is to take advantage of the fact that ROOT
   is able to automatically parse nested classes into a tree structure. Something like
   \code
   class Sub {
     double a;
     double b;
   };

   class Top {
     sub sub;
     double c;
   };

   Top top;
   \endcode

   Gets converted to the following tree structure:

   \code
               top
              /   \
             /     \
            |       |
           sub      c
           / \
          /   \
         |     |
         a     b

  \endcode


   \section data_skel Skeleton

   Once you've got a compatible set of analysis codes written, then there's a few things you need to do
   to make it work with ROOTBEER. All of the files that you'll need to edit are located in the \c user directory.
   This directory is also the place where you can locate the source code files for your unpacking routines, if you
   want (you can also have them located anywhere on you system, as exaplained below). The files you'll definitely
   need to edit are <tt>Makefile.user</tt>, <tt>UserLinkdef.h</tt>, and <tt>Skeleton.cxx</tt>.
  
   The first thing you need to do is to tell the compiler and CINT about your source files and their locations.
   First, open the \c Makefile.user file, and fill in the blanks to define the directory in which your header
   files are located (if it's not <tt>rootbeer/user</tt>), and the paths to each of your source and header files.
   The comments in \c Makefile.user explain how to do each of these things.  Somewhat unrelated, you'll also need
   to specify the type of data that is present in your packed event files (i.e., short int, int, long int, etc.),
   by filling in the appropriate value of the <tt>DATATYPE</tt> variable at the bottom of the file.

   After editing <tt>Makefile.user<\tt>, open up <tt>UserLinkdef.h</tt>. Again, the comments will tell you what
   to do (or for a more readable version, see the linked doxygen page).  The basic idea is that you're telling
   CINT, ROOT's C++ interpreter to generate a dictionary for your classes, so that you can work with them interactively
   in the interpreter.

   The last file you need to edit is <tt>Skeleton.cxx</tt>. Again, most everything is explained in the comments, but
   we'll go through it here too. The first thing you need to do is to \c #include the header files for any of the code
   you've written.  Do this under the section denoted with
   \code
   //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
   //\\ Add the header includes for your classes here. //
   //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
   \endcode
   Below that \c #include all of the headers you'll need.
   \code
   #include "MyFirstHeader.hxx"
   #include "MyNextHeader.hxx"
   ...
   \endcode

   The next thing you'll need to do is to create some instances of your user data classes. This is done below the
   comment block
   \code
   //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
   //\\ Define instances of your class here    \\//
   //\\ (using the ADD_CLASS_INSTANCE macros). \\//
   //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
   \endcode
   using the \c ADD_CLASS_INSTANCE macros. For more information on using these macros, see the docuentation page
   on Skeleton.hxx

    Finally, you need to implement the routines to do the actual buffer extraction and processing.
    This is done by filling in the blanks below
    \code
    //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
    //\\ Here you should define how to process your data buffers        \\//
    //\\ by implementing the ReadBuffer() and UnpackBuffer() functions. \\//
    //\\ For more information, see the doxygen page on rb::unpack       \\//
    //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
    \endcode

    The first function you'll need to fill in is ReadBuffer() (see the linked dxygen page for more info on what
    this function is suposed to do). The basic idea is that you want to tell ROOTBEER how to take the raw data from a
    <a href = http://www.cplusplus.com/reference/iostream/istream/>std::istream</a> and copy it over into an
    appropriately-sized <a href = http://www.cplusplus.com/reference/stl/vector/>std::vector</a>.  Routines to
    do this are already written for NSCL and MIDAS data, so if applicable you can use these routines by uncommenting the
    appropriate \c #define macros.  Otherwise, you'll need to write your own (in the \c #else conditional compilation
    block).

    The next function to fill in is UnpackBuffer() (again, follow the link for more info).  This function tells ROOTBEER
    how to process the data in each buffer. Usually, you'll delegate the bulk of this to routines in your user code, but unless
    we start making assumptions about how your data is strucured, this function will probably need to include
    a bit of data crunching directly.  Really, it's hard to say much more about what to do with this function without
    knowing what your data looks like, so the rest is up to the user. Do keep in mind that, as noted with a comment block,
    you need to include a call to rb::Hist::FillAll() after you have processed each event, otherwise your data will never
    make it into your histograms which defeats the whole purpose.

    One thing to note about implementing UnpackBuffer() is how to get access to the user classes you instantiated
    using <tt>ADD_CLASS_INSTANCE</tt>. Because your data will be unpacked in a thread separate from the main one running
    CINT, access to it needs to be controlled by mutexes (if that doesn't make any sense, don't
    worry, just keep reading). In fact, things have been set up so that the only straightforward way you can get access to your
    class instances is through a LockingPointer. You can do this easily using the \c GET_LOCKING_POINTER macro, which takes
    three arguments
    -# The pointer variable name.
    -# The original variable name you used in \c ADD_CLASS_INSTANCE
    -# The class name.

    For example, if I want access to \c par as created a few lines above:
    \code
    GET_LOCKING_POINER(pPar, par, MyParams);
    \endcode
    Now I've access to the data and routines of \c par via the LockingPointer <tt>pPar</tt>. You can learn more about how to
    use the LockingPointer in it's class documentation, but the three most useful functions are:

    the dereference operator
    \c ->
    \code
    pPar->SomeMemberFunction();
    \endcode
    the indirection operator \c *
    \code
    // elsewhere
    void SomeFunction(const MyParams& p);
    // here
    SomeFunction(*pPar);
    \endcode
    and the \c Get() function, which returns a pointer
    \code
    // elsewhere
    void SomePointerFunction(MyParams* p);
    // here
    SomePointerFunction(pPar.Get());
    \endcode


   For more information on what's going on behind the scenes in what's been described in this section, see the
   documentation on the rb::Data class.


   \section data_other Other

   Once you've got your analysis codes written and Skeleton.cxx is filled in, you should be ready to try compiling
   your customized version of ROOTBEER
   \code
   make clean
   make
   \endcode

   Successful compilation will create the \c rootbeer executable, along with some shared libraries (in <tt>/lib</tt>).
   Try running it by typing <tt>./rootbeer</tt>

   If you notice any difficulties that you thing were the fault of the ROOTBEER design, or with this documentation,
   feel free to contact
   <a href = mailto:gchristian@triumf.ca>Greg</a> with comments, complaints, or suggestions.  As much as possible, we
   hope that adapting ROOTBEER to work with your experiment will be a painless experience, and we take suggestions on how
   to improve the ROOTBEER design and the interface at the user level very seriously.
   Of course, we also reserve the right to ignore you if we don't like what you have to say....

  \page develop Developers
  Developers....







*/
