//! \file main.cc
//! \brief Implements the \c main program and Doxygen documentation.
#include <algorithm>
#include "Rint.hxx"


/// \brief The \c main ROOTBEER function.
//! \details Creates an instance of \c rb::Rint and runs it.
Int_t main(Int_t argc, Char_t** argv)
{
  bool lite(false);
  for(int i=1; i<argc; ++i) if(!strcmp(argv[i],"-l")) lite = true;
  rb::Rint rbApp("RootBeer", &argc, argv, 0, 0, lite);
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
  The ROOTBEER source code can be downloaded as a .zip file from https://github.com/gchristian321/rootbeer
  (click the "ZIP" link on the left side of the page). The sources can also be checked out directly using
  git, with the appropriate paths listed on the github webpage.

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
  one (data saved in a file). Attaching to data sources is done in a separate thread from the one running CINT, so
  you are able to have access to the command line while data are acquired, unpacked, and put into histograms behind
  the scenes.

  To attach to an online data source, the command is
  \code
  rb::AttachOnline(const char* source, const char* other, char** others, int nothers);
  \endcode
  The specific arguments to rb::AttachOnline() will depend on your experiment since we can't know <i>a priori</i>
  what information is needed for you to attach to online data. Typically "source" refers to some host machine,
  "other" is any other information that might be needed, and then the remainind arguments can be used in case two
  fields are not sufficient.

  Offline data sources can be read using
  \code
  rb::AttachFile(const char* filename, bool stop_at_end);
  \endcode

  The first argument is the name (path) of the offline file, and the second specifies whether you want to stop
  reading at the end of the file [true], or stay attached and wait for more data to come it [false].

  It's also possible to string multiple offline files together using
  \code
  rb::AttachList(const char* filename);
  \endcode

  Here the argument is the name of a file that lists the path of each data file you want to read.  This file
  can also contain comments, marked by <tt>#</tt>, and all whitespace is ignored.

  Any time you are attached to a data source (any type), you can cancel using the function
  \code
  rb::Unattach();
  \endcode

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

  In addition to the standard 1-, 2-, and 3-dimensional histograms, ROOTBEER also includes some specialized
  histogram types. For more information, consult the linked documentation for each:

  
  - "Gamma Histograms": rb::GammaHist
  - "Summary Histograms": rb::SummaryHist
  - "Bitmask Histograms": rb::BitHist



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

  as well as zero out ("Clear") the histogram currently drawn in a canvas

  \code
  rb::Canvas::ClearAll(); // Clear all canvases

  rb::Canvas::ClearCurrent(); // Clear just the currently selected canvas
  \endcode

  Note that the rb::Canvas::Clear* functions are distinct from the standard ROOT TCanvas::Clear() function,
  which removes the histogram from the canvas entirely.




  \page gui Graphical User Interface
  \todo Write GIU section


  \page data Customizing for Your Experiment

  - <a href=#data_intro><b>Introduction</b></a>
  - <a href=#data_skel><b>Adding Your Classes to ROOTBEER</b></a>
  - <a href=#data_buffers><b>Defining How to Acquire and Unpack Data</b></a>
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
     Sub sub;
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


   \section data_skel Adding Your Classes

   Once you've got a compatible set of analysis codes written, then there's a few things you need to do
   to make it work with ROOTBEER. All of the files that you'll need to edit are located in the \c user directory.
   This directory is also the place where you can locate the source code files for your unpacking routines, if you
   want (you can also have them located anywhere on you system, as exaplained below). The files you'll definitely
   need to edit are Makefile.user, UserLinkdef.h, ImportData.h, and User.cxx
  
   The first thing you need to do is to tell the compiler and CINT about your source files and their locations.
   First, open the \c Makefile.user file, and fill in the blanks to define the directory in which your header
   files are located (if it's not <tt>rootbeer/user</tt>), and the paths to each of your source and header files.
   The comments in \c Makefile.user explain how to do each of these things.

   After editing <tt>Makefile.user</tt>, open up <tt>UserLinkdef.h</tt>. Again, the comments will tell you what
   to do (or for a more readable version, see the linked doxygen page).  The basic idea is that you're telling
   CINT, ROOT's C++ interpreter to generate a dictionary for your classes, so that you can work with them interactively
   in the interpreter and so that they can automatically be parsed into a tree structure.

   Next, you will need to define (or "import") the instances of your user class that you need in rootbeer. To do this,
   open up the ImportData.h file, and use the RB_IMPORT_DATA macro to add instances of your classes:
   \code
   RB_IMPORT_DATA(ClassType, variableName, "treeBranchName", (bool)visible, "constructor args");
   \endcode
   See the linked documentation page on
   <a href="rb__import__data_8h.html#a7623fcbdf2c95113416d11bc0fd5286e"><b>RB_IMPORT_DATA</b></a>
   and ImportData.h for more information and examples. It should be noted that the RB_IMPORT_DATA macro does not
   create a "bare" instance of your class directly. Instead it creates a new global instance of rb::Data <T>, with
   the template argument being your class type. Doing things this way automatically calls the appropriate functions
   allowing your data to be displayed in histograms, and furthermore it allows thread-safe access to the classes,
   both in compiled code and in CINT.  For more information, see the documentation on rb::MData and rb::Data.


   \section data_buffers Acquiring and Upacking Data

   Now that you've got instances of your data classes added to ROOTBEER, you'll need to tell it how to acquire
   and unpack data.  This is done by defining a new class that inherits from the abstract base class rb::BufferSource
   and implements all of its pure virtual methods. Additionally, you will need to implement the static New() method
   of rb::BufferSource such that it returns a pointer to a new instance of your derived class.  The definition and
   implemention of your derived class and rb::BufferSource::New() can all be contained in the file User.cxx. More
   detailed instructions on how to create your derived BufferSource class can be found in the documentation pages of
   both rb::BufferSource and User.cxx.

   If your experiment involves MIDAS or NSCL (spectrodaq) readout, then you may be able to make use of the pre-defined
   BufferSource derived classes that come with stock ROOTBEER. In principle, if you use either of these two systems,
   you will only have to implement the UnpackBuffer() method (which will always be experiment specific).  To "turn on"
   the use of either of these pre-defined classes, uncomment the appropriate lines as noted in UserLinkdef.h

   In the course of implementing your BufferSource derived class, you'll presumably want access to the global instances
   of your user classes (otherwise you don't have anywhere to put the data).  Direct access to your classes is not allowed
   due to the issues that could arise in a threaded environment.  Instead, you'll need to use the GetPointer() method
   of rb::Data to obtain a special type of pointer that allows thread safe access to your data. This special pointer
   (AutoLockingPointer), has the standard pointer semantics, e.g. a dereference and indirection operator
   \code
   p->SomeMemberFunction(); //< dereference
   SomeFunction(*pPar); //< indirection
   \endcode

   Consider the following example:
   \code
   // [ MyHeader.hxx ] //
   struct MyClass {
     double d;
     void f();
   };
   void ff (const MyClass& c);

   // [ ImportData.h ] //
   #include "MyHeader.hxx"
   RB_IMPORT_DATA(MyClass, gMyClass, "mine", false, "");

   // [ User.cxx ] //
   #include "MyHeader.hxx"
   class MyBufferSource : public rb::BufferSource {
     // ... //
   };

   Bool_t MyBufferSource::UnpackBuffer() {
     AutoLockingPointer pMyClass = gMyClass->GetPointer();
     pMyClass->f();
     ff(*pMyClass);
     // ... //
   }

   rb::BufferSource* rb::BufferSource::New() {
     return new MyBufferSource();
   }
   \endcode


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
