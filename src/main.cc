//! \file main.cc
//! \brief Implements the \c main program and Doxygen documentation.
#include <set>
#include <TROOT.h>
#include "Rint.hxx"
#include "Rootbeer.hxx"


#ifndef RB_UNPACK_ONLY

/// \brief The \c main ROOTBEER function.
//! \details Creates an instance of \c rb::Rint and runs it.
int main(int argc, char** argv)
{
	std::set<std::string> args(argv, argv+argc);
	Bool_t lite = args.count("-l");
  rb::Rint rbApp("Rootbeer", &argc, argv, 0, 0, lite);
  gROOT->ProcessLine("gStyle->SetOptTitle(kTRUE)");
  gROOT->ProcessLine("gStyle->SetOptStat(kTRUE)");
  rbApp.Run();
  return 0;
}


#else
#include <cassert>

namespace {
void usage() {
	std::cout << "usage: rbunpack <input file>\n\n";
	exit(1);
}
void handle_args(int argc, char** argv, std::string& fin) {
	if(argc != 2) usage();
	fin  = argv[1];
} }

/// \brief Main function for a separate program to convert data files to root trees.
int main(int argc, char** argv)
{
	std::string fin;
	handle_args(argc, argv, fin);
	assert(strlen(argv[0]) > 3);
	sprintf(argv[0], "-ng");
	rb::Rint rbApp("Rootbeer", &argc, argv, 0, 0, true);
	rbApp.StartSave(false);
	rb::AttachFile(fin.c_str());
	gSystem->Sleep(1e2);
	while(TThread::GetThread("AttachFile"));
	rbApp.Terminate(0);
}



#endif



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
  may need to be implemented as well (currently <a href="http://docs.nscl.msu.edu/daq/bluebook/html/" target="_blank">
	NSCL/MSU</a> and <a href="https://daq-plone.triumf.ca/SR/MIDAS/" target="_blank">MIDAS</a> online data are supported).

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
  reading at the end of the file [true], or stay attached and wait for more data to come in [false].

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
	The syntax to work with a rootbeer histogram in CINT is very similar to that of normal ROOT histograms. Nearly all
	of the TH1 member functions have been transferred to the rootbeer histograms and should work the same (you can find
	a complete list of member functions in the rb::hist::Base documemtation).


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

  
  - "Gamma Histograms": rb::hist::Gamma
  - "Summary Histograms": rb::hist::Summary
  - "Bitmask Histograms": rb::hist::Bit



  \n \section canvas Canvases

  In general, you can work with <a href="http:://root.cern.ch/root/html/TCanvas.html" target="_blank">TCanvas</a>
  objects just as you would in normal ROOT.  However, ROOTBEER adds a bit of functionality relevant to the display
  of streaming data. The main addition is the ability to have ROOTBEER auto-refresh your canvases on a regular basis. 
  To Start/Stop auto-updating use the StartUpdate() or StopUpdate() functions:

  \code
  rb::canvas::StartUpdate(5); // All canvases will now refresh every five seconds.

  rb::canvas::StopUpdate(); // Canvases will no longer auto-refresh
  \endcode


  You can also update canvases manually, either individual or all at once
  \code
  rb::canvas::UpdateAll(); // Update all canvases

  rb::canvas::UpdateCurrent(); // Update just the currently selected canvas
  \endcode

  as well as zero out ("Clear") the histogram currently drawn in a canvas

  \code
  rb::canvas::ClearAll(); // Clear all canvases

  rb::canvas::ClearCurrent(); // Clear just the currently selected canvas
  \endcode

  Note that the rb::canvas::Clear* functions are distinct from the standard ROOT TCanvas::Clear() function,
  which removes the histogram from the canvas entirely.




  \page gui Graphical User Interface
	
	Below is a basic screenshot of the rootbeer graphical user interface (GUI). In this section, we'll go over
	what the various buttons do and how you can use them.

	\image html rbeer_screenshot.png "The rootbeer gui."

	\section data_gui Data Frame
	
	The "main" (leftmost) gui page allows control over data sources and canvas displays.  The upper frame (labeled
	"Data") gives control over data sources.  The buttons on the left of the frame allow users to attach to various
	sources of data.  The top button ("Attach Online") connects to real-time online data, and the text entry fields
	"Host" and "Expt" allow the user to specify where to look for incoming data buffers. The purpose of the text boxes
	is somewhat specific to MIDAS experiments, where online sources are specified by a host computer and a MIDAS experiment
	running on said host. However, users of other systems will likely need to specify similar parameters, so the boxes are
	still useful (hopefully).

	The second button from the top ("Attach File") lets users attach to a file containing saved run data. Clicking on the
	button opens up a dialog box, from which the user selects the file he/she wants to scan (note that the default directory
	to which the dialog box points is set at compile time; please see the <a href=data.html>Customizing for Your Experiment</a>
	page for more information
	on how to set this).  The "Continuous" check box to the right of the button determines what the program should do when the
	end of the run file is reached. Default behavior (unselected) is to assume the run is complete and stop looking for data, i.e.
	we're finished with the run. The other alternative (selected check box) is to stay attached to the file and wait for more
	data to come in. This can be useful in cases where you want to use a file as a buffer between the online data and your
	analysis.  With this option selected, whenever the program reaches the end of a run file, it waits 10 seconds and then checks
	if more data has been appended to the run. If so, it continues to analyze the new data; otherwise the wait cycle is repeated
	until the user manually unattaches from the file.

	The third button from the top ("Attach List") allows users to scan a list of offline run files in sequence.  Like "Attach File",
	it brings up a dialog box from which the user can select the file he/she wants to load. In this case, what needs to be selected
	is a text file that specifies the paths of all of the run files the user wants to scan. In this "list" file, the path
	of each desired run should be on a separate line; whitespace is ignored, as is anything between a comment marker \c # and 
	the end of a line. Example:
	\code
	# LIST of MIDAS Files to analyze

	/path/to/somewhere/run1.mid # run 1
	/path/to/somewhere/run2.mid # run 2
	/path/to/somewhere/run3.mid # run 3
	# /path/to/somewhere/run4.mid # run 4
	## ^ Decided I don't really want to look at run4

	/path/to/somewhere/run5.mid
	\endcode
	

	The bottom ("Unattach") button allows the user to stop analysis whenever attached to a data source of any type. This button
	is only active when attached to a data source.

	The "Save Data" and "Save Histograms" checkboxes allow users to save the data
	that the read in into a root file.  Checking "Save Data" saves all event-by-event data in root TTrees, and checking
	"Save Histograms" (only available when "Save Data" is already checked) also saves any histograms created in the current
	session to disk.  When selecting a save option, the output file path is automatic (set at compile time similar to the search
	path for attaching to files).  If attaching to offline (file) data, then
	the data from each individual run file will be saved in a separate .root file; the name of the .root file will be identical
	to that of the run file, except with the extension replaced by \c .root (so \c run1.mid becomes <tt>run1.root</tt>). Note that
	when selecting "Attach List", a new .root file is created for each run file in the list.  In the case of online data, a new
	.root file is created any time the user re-attaches to the online source, and the file name is automatically set from the date
	and time the run occurred.

	\section canvas_gui Canvas Frame

	The lower frame on the "main" gui window, labeled "Canvas" allows control over canvases in the current rootbeer session.
	Rootbeer doesn't use any special or custom canvases for displaying data; rather, it uses stock ROOT canvases with a few
	extra functions.  The topmost row in the canvas frame allows the user to control the canvas auto-refresh rate. Clicking the
	"Start Refresh" button begins to auto-update all active canvases in the session, and the number selection box to the right of
	the button allows the user to control the refresh rate. When in refresh mode, every canvas or sub-pad containing a rootbeer
	histogram will re-draw the histogram every \c n seconds, where \c n is the selected refresh rate. Note that when auto-refresh
	is turned on, the "Start Refresh" button will become "Stop Refresh" (and change color to red); in this case, clicking it turns
	off the auto-refresh utility.

	The next row of buttons "Refresh Current"/"Refresh All" allows the user to refresh canvases independent of the auto-refresh
	utility. "Refresh Current" refreshes only the currently selected Canvas or Pad, while "Refresh All" refreshes every Canvas/Pad
	in the program.  Similarly, the "Zero Current" and "Zero All" buttons allow the user to zero-out the histograms displayed
	either in the currently selected Canvas/Pad or all of them.  Here, "zero-out" means that the content of each bin of a histogram
	is set to zero.

	The lowermost rows of buttons on the canvas frames control some commonly used Canvas functions that are already part of stock
	ROOT. "Divide Current" allows the user to divide the curently selected canvas into subpads; clicking it brings up a dialog
	box from which the user can select the division geometry (e.g. 2x2, 2x3, 3x3, etc.).  "Cd" allows the user to change the 
	currently selected canvas; it also brings up a dialog box containing a drop-down menu from whch the user can select the new
	canvas (or sub-pad) to which he/she wants to switch.  "Clear Canvas" clears the currently selected Canvas/Pad. Note that
	"Clearing" a canvas is different from "Zeroing" - where zeroing simply sets the bin content of histograms to zero, clearing
	completely removes the histogram (and any other graphical objects, including sub-pads) from the canvas.  Finally, the
	"Create New" button allows the user to make new canvases, and the "Name" text entry box to the right of it specifies the
	name of newly created canvases (in the case of duplicate names, _1, _2, etc. will be appended to make the names unique,
	and if nothing is entered a default name will be given to the new canvas).

	The "Configuration" sub frame allows the user to save or load canvas configuration files.  A canvas  configuration file
	contains all of the macro code needed to reproduce the canvas environment at the time of the save; that is, it will
	re-construct any canvases present, divide them into the same number of sub-pads, and re-draw whatever histograms were
	present in the canvas at save time.


	\section gui_hist Histogram Frame

	The right window in the screenshot allows users to create and manipulate histograms and directories. The "Create New"
	frame provides a graphical means of creating new histograms. The various fields are hopefully somewhat intuitive,
	but I'll go through them briefly:

	- Type: using this drop-down menu, select the histogram type (1/2/3-dimensional, summary, "gamma", bitwise; for
	information on the various histogram types see the rb::hist::Base class documentation and links to the respective
	derived types)
	- Name: Enter the histogram name (required).
	- Title: Enter the histogram title (optional). If left blank, a default title is assigned specifying the parameter and
	gate conditions.
	- Parameter: From the drop down menu, select the TTree parameter that you want to histogram on the indicated axis. 
	Note that the "y" and "z" axes only become active for histogram types where appropriate.
	You can also choose to histogram	functions depending on TTree parameters by manually typing in the box
	(for example, I could type <tt>gamma.bgo.q[0] * gamma.bgo.q[0]</tt> to look at the square of
	<tt>gamma.bgo.q[0]</tt>).
	- Bins, Low, High: Select the number of bins, low edge and high edge of the specified axis.
	- Gate: Here you can set the gate condition for your histogram by filling in the text box; the gate condition must
	be a logical statement depending on relevant TTree parameters and constants (non-rectangular gate conditions can be
	set by creating a <a href=http://root.cern.ch/root/html/TCutG.html>TCutG</a> and then setting its name as part of the
	logic statement). For a given event, each histogram is
	only filled when its gate condition evaluates to true.  Note that leaving the gate field empty makes the histogram
	in question "ungated", i.e. it will be filled for all events.
	- Event Type: This allows the user to select the type of event that is being histogrammed. Each "event type" is
	associated with a single root TTree, and selecting a different event type from the drop down menu causes the "Parameter"
	drop down menus to be re-populated. The exact format of the various events and parameters in your experiment will depend
	on the implementation of the person who designs your analysis code.

	The "Create/Replace" buttons either create a new histgram or replace a current one from the selections you have made
	in the list above. If no histogram is currently selected in the window to the left of the "Create New" frame (more on this
	in  bit), then the button takes on its "Create" functionality; that is, it creates a brand new histogram without affecting
	any that are already in existence. If a histogram is selected in the window, then the "Create/Replace" button takes on it's
	"Replace" functionality - it deletes the selected histogram and then replaces it with a new one.
	
	It is also possible to change the gate condition on any currently existing histogram.
	To do this, select (single click) the desired histogram in the window to the left, enter the new gating conditin in the
	"Gate" text box, and then click the "Regate" button.

	This large menu at the left of the frame allows the user to view and manipulate already existing histograms and directories.
	As you can start to see in the screenshot, this window displays the current TDirectory structure and any histograms
	created in the present rootbeer session.  Single clicking on a directory (folder icon), causes that directory to become
	the "currently focused" one, i.e. it issues a \c gROOT->cd() command.  Single clicking on a histogram (graph icon)
	makes the corresponding histogram the "selected" one. This means that the "Create/Replace" and "Regate" buttons will
	act on the selected histogram as described above, as will the "Draw", "Delete", and "Command" buttons/entries.

	The "Draw" button draws the currently selected histogram, either in the currently selected canvas (if there is one) or in
	a new canvas window (if one isn't already open).  The "Draw option" text entry box allows the user to select the draw
	option argument (for some examples of draw options, see
	<a href=http://root.cern.ch/root/html/tutorials/hist/h1draw.C.html>
	http://root.cern.ch/root/html/tutorials/hist/h1draw.C.html</a> and 
	<a href=http://root.cern.ch/root/html/tutorials/hist/draw2dopt.C.html>
	http://root.cern.ch/root/html/tutorials/hist/draw2dopt.C.html</a>).  Note that double clicking on a histogram in the
	window is identical to first single clicking on it, then clicking the "Draw" button.

	As might be expected, the "Delete" button deletes (removes from rootbeer completely) the selected histogram or directory.
	In the case of deleting a directory, all histograms and sub-directories are also removed.

	The "Command" text entry box allows the user to call rb::hist::Base member functions directly from the gui
	(click on the rb::hist::Base link to see a list of all available functions). To use, enter
	a valid member function syntax in the box and either hit "Return" or click "OK". The member function is performed on the
	currently selected histogram (if there is one). For example, to change the line color of the selected histogram to blue,
	type:
	\code
	SetLineColor(kBlue);
	\endcode

	In the box and press "Return" or click "OK".

	The "New Directory" button to the right of the "Draw Option" box allows the user to create new TDirectories; the
	newly created directory will be inserted as a sibdirectory of the one currently selected in the window. The
	"Configuration File" subframe within the "Create New" frame allows saving/loading of histogram configuration files,
	which will be explained more in the <a href=#gui_variables>next</a> section.	Finally, the
	"Quit" button in the lower-right corner of the frame exits the rootbeer program completely, without saving any data.

	\warning  Currently, clicking buttons with potential negative consequences (Delete, Quit), there is no
	checking if you "really want to do this", so click with care!



	\section gui_variables Variables Frame

	In the last section, you may have noticed that the right window has two tab selction options. The figure below shows
	what things look like with the alternate ("Variables / Configuration") tab selected.

	\image html rbeer_variables_screenshot.png "Variables/Configuration tab."

	The main scroll window on the left of the frame displays all of the available variables in your experiment, arranged
	in a tree structure. Note that by "variables", we mean quantities utilized in the user's analysis code - things like
	calibration slopes/offsets, channel mappings and the like.
	Single clicking on a variable makes it the "currently selected" one. Clicking  the "Read Value"
	button populates the text box at the bottom of the frame with the variable value (note that double clicking on a
	variable has the same effect). Changing the value in the text box and then clicking "Set Value" changes the value
	of the selected variable. This change is a "live" one, i.e. any future analysis depending on the value of the
	selected variable will be performed using the new variable value.

	The "Configuration Files" frame on the right allows users to save and load a variety of configuration files.
	Each button opens a pop-up window to select the file that you want to save or load.  "Save Histograms" generates
	macro code to reproduce the directory structure, histograms, and TCutGs present at save time
	(this is identical to the "Save" button on the histograms frame).  "Save Variables" has a similar effect,
	except is produces code to reproduce all of the variables and their values at save time. "Save Canvases"
	saves the canvas structure as
	<a href=#gui_canvas>described previously</a>.  "Save All" generates a macro file reproducing histogram, variable,
	and canvas configurations at save time.  Finally, the "Load" button allows the user to read in a previously
	saved configuration file. By changing the "Load Method" selction to its left, the user can choose what to do in the
	case of overlaps between currently present histograms and
	<a href=http://root.cern.ch/root/html/TCutG.html>TCutG</a>s and those in the loaded file with the same name.
	"Reset" completely resets everything, deleting all histograms and TCutGs before loading the file. "Overwrite" overwrites
	any existing duplicates but leaves all others alone.  "Cumulate" leaves all currently exitsing objects the same, and
	if there are duplicates in the configuration file, the newly created object will have _1, _2, etc. appended to its
	name until it is unique.  Note that objects other than histograms and TCutGs are always loaded from the new file; for
	example, when loading a file containing variable definitions, variable values will be set from the loaded file regardless
	of the method selected.



  \page data Customizing for Your Experiment


  - <a href=#data_prereq><b>Prerequisites</b></a>
  - <a href=#data_intro><b>Introduction</b></a>
  - <a href=#data_event><b>Creating Event Classes</b></a>
  - <a href=#data_wrapper><b>Wrapping your Experiments Classes</b></a>
  - <a href=#data_buffer><b>Adding a New Type of Buffer Source</b></a>
  - <a href=#data_orginization><b>Orgainzing and Compiling your Code</b></a>

 \section data_prereq Prerequisites

  For users who are new to coding in C++, we'll briefly go over a few of the concepts that you will need to
	understand before attempting to integrade your analysis codes with ROOTBEER. This is in no way intended to be
	a comprehensive overview, just a quick introduction with some links to online documentation that you might find
	useful. You will also need to be comfortable with some of the basic concepts found in C/C++, such as basic
	keywords and the use of <a href=http://www.cplusplus.com/doc/tutorial/pointers/>pointers</a>.
	If these concepts are new to you, it would be a good idea to become familiar with them before attempting
	to develop code to be used with ROOTBEER.	Experienced C++ developers can safely skip to the
	<a href=#data_intro>next section</a>.

	First, you'll need to be familiar
	with classes, which are basically coder-created objects which can contain ("encapsulate") data (integers, floating
	points, arrays, vectors, other classes, etc.) and functions.  In C++ jargon, data contained within a class are often
	referred to as "data members," and functions are often called "member functions".  Classes can also make use
	of keywords ("private", "protected", and "public") to control the scope of data members and member functions. You can
	find more information on classes (including an overview of the scope keywords) at the following link:
	<a href=http://www.cplusplus.com/doc/tutorial/classes/>http://www.cplusplus.com/doc/tutorial/classes/</a>

	A concept reated to classes is "inheritance." The basic idea here is that the coder can first create a "parent" class
	which has a certain set of member data and member functions. Next, he/she creates a "child" or "derived" class which
	inherits all of the data and functions from the parent class, possibly adding some more of its own. By "inherits", we
	mean that the derived class gets access to the parent class's data and functions as if they were part of the child
	class's definition in the first place (with some caveats regarding the scope keywords mentioned earlier).

	One feature of C++ inheritance is the ability to create "virtual" member functions in parent classes. Virtual member
	functions are similar to normal ones, except that derived classes have the option to override the implementation given
	by the parent class. For example, we could create a parent class with a virtual \c print function that prints "I am a
	parent" to the screen. Then in a derived class we could re-implement ("override") the \c print function to say "I am
	a child" instead.  Virtual functions can also be "pure virtual" which means that they are not given any implementation
	in the parent class. This means that any derived classes are required to privide an implementation of the pure virtual
	functions.

	Virtual functions introduce the concept of "polymorphism", which allows for the creation of a framework as is used
	in ROOTBEER.  The basic idea behind polymorphism is that the coder can create a parent class, provide it with some
	(possible pure) virtual functions, and then write additional code that uses those virtual functions. Later, one can
	create a derived class which provides a specific implementation of the parent class's virtual functions, and use this
	specific class in the other areas of the code. Thus the final result will then depend on the specific implementation
	of the virtual functions in the derived class.  This is the basic idea behind the ROOTBEER framework. We (the developers)
	have created some parent classes with virtual functions, and then called those virtual functions in other areas of the
	source code.  You (the user) will need to create derived classes inheriting from our parent classes and implementing the
	various virtual functions.  The final result is that data are sourced and processed depending on the specific structure
	and needs of your experiment.

	As mentioned, this section was only a very brief overview to the concepts covered. For more information, you can check
	out the <a href=http://www.cplusplus.com/doc/tutorial/classes/>link</a> we already mentioned. The topics covered under
	the <i>Object Oriented Programming</i> section give a more detailed introduction to these concepts.


  \section data_intro Introduction

  ROOTBEER is intended to be a framework into which you can plug the analysis code for your experiment.
  In other words, you write the code to unpack and analyze the data from your detectors and ROOTBEER handles
  the display side of things, putting your data into histograms and so on.  As such, we've tried to set up a
  framework that is very general and puts few constraints on how your analysis code must be written. ROOTBEER
	takes heavy advantage of ROOT's ability to parse user-defined classes into a tree structure, creating TTree
	branches and leaves based on the nested structure of your classes. For example, if we define some nested
	classes,

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

   ROOT will convert them into the following tree structure:

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



	The use of ROOT's tree-creation facilities places a few requirements on how you structure the data in your
	analysis codes:

	-# It must be in C++ or "forward compatable" C (that will compile with a C++ compiler). <br><br>
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
  won't. <br><br>
	-# Any array structures that you wish to include in a tree must be defined as plain C arrays (and not something like
	<tt>std::vector</tt>). Currently, only plain arrays can be properly parsed into a tree structure as intended (note that
	it \e might be possible to add support for standard library containers, but we haven't looked into this).


	Also, as a general note, keep in mind when designing your analysis classes that their names and the way you nest them
	will affect the branch naming scheme in the TTrees generated by ROOT (see the example above to get an idea). Thus you
	may want to design and name your classes such that you end up with branch names that will make sense to a typical user
	of your codes. Also, you will likely need to add specific comment fields behind your data members to control how
	they are included into the tree structure (more on this in the next section).
	

	\section data_event Event Derived Classes

	To tell rootbeer how to handle a specific experiment's data, users must design and implement classes which inherit
	from rb::Event.  The idea that each derived class will correspond to a different type of event that might be generated
	in the experiment. For example, one derived class might correspond to an event packet from a detector sub-system, another
	to a scaler packet, and so on. The precise structure is, of course, up to the user.  Each derived class needs to implement
	the various pure virtual functions of rb::Event to tell rootbeer precisely how to handle an event of that type.  Typically,
	the user will want to encapsulate his/her data classes within the derived class (but wrapped by rb::data::Wrapper - see the
	next section), so that the event data can be unpacked into the user's data classes (and by extension, into root trees and
	histograms).

	Once event classes have been created, they will need to be "registered" with the main rootbeer program. This is done by
	implementing the function rb::Rint::RegisterEvents(), which has been left empty in the stock version of rootbeer. Each event
	class can be registered by simply adding a line within the function:
	\code
	RegisterEvent<Type>(Code, Name);
	\endcode

	Here the template argument \e Type is the class being registered, \e Code is an integer assigning a unique code to this
	specific event class, and \e Name is a string assigning a unique name to this specific event class.  Registering an event
	creates a singleton instance of the specific rb::Event-derived class in question.  This instance can be accessed in one
	of two ways:
	
	-# The rb::Event::Instance static template function
	\code
	rb::Event::Instance<Type>();
	\endcode
	<br><br>
	-# The rb::Rint::GetEvent function, which makes use of the unique event code assigned in RegisterEvents()
	\code
	rb::Rint::GetEvent(Code);
	\endcode

	Note that each of these options returns a pointer to rb::Event, which only allows access to functions defined
	in the parent class. If specific derived class functions are needed, the pointer will need to be downcasted to
	the appropriate type.

	Also note that option (2) above is not a static function - a specific instance of rb::Rint is needed to call it.
	In rootbeer (actually root in general), there is a global instance of rb::Rint, which can be accessed by the function:
	\code
	rb::gApp()
	\endcode
	which returns a rb::Rint pointer. The global rb::Rint instance can also be accessed by downcasting root's native
	\c gApplication pointer from <tt>TApplication*</tt> to <tt>rb::Rint*</tt>.


	\section data_wrapper User Class Wrappers

	As mentioned in the previous section, you will generally want to encapsulate your data classes within  the
	rb::Event-derived classes that you create. This will result in a single TTree being generated for each type
	of event.  In order to properly generate the trees and parse them into branches, you should wrap your classes
	within the template rb::data::Wrapper class. This means that instead of creating an instance of your class directly,
	\code
	MyClass mine;
	\endcode
	you'll introduce an instance of the template rb::data::Wrapper class, with your class as the template argument:
	\code
	rb::data::Wrapper<MyClass> mine;
	\endcode
	The constructor of rb::data::Wrapper takes some specific arguments that tell ROOTBEER how to handle your class:
	\code
	rb::data::Wrapper<T>::Wrapper(const char* name, Event* event, bool makeVisible, const char* constructor)
	\endcode

	\e name is a string assigning a branch name to your class; \e event is a pointer to the rb::Event-derived class
	associated with the class instance (since typically you'll be calling the constructor from within the rb::Event-derived
	class, you can use \c this here); \e makeVisible determines whether or not you want the class members accessable from
	CINT; and \c constructor is a string specifying the exact form of the class constructor to call (<tt>""</tt> corresponds
	to a default constructor).  You can get a bit more information by checking out the constructor documentation on
	rb::data::Wrapper.
	As a brief example, say we have a user class <tt>MyClass</tt> that we want to use in handline a specific type of event.
	In the event class definition, we might have something like:
  \code
  class MyEventClass : public rb::Event {
  private:
    rb::data::Wrapper<MyClass> fMyClass;
  public:
    // ...More Class Definition Here... //
  };
  \endcode

	Then in the implementation of the class constructor, you would call the rb::data::Wrapper constructor:
	\code
  MyEventClass::MyEventClass : fMyClass("mine", this, true, "") {
    // ... //
  };
	\endcode

	The above code would generate a TTree branch with the top level name \c "mine" (<tt>mine.branch.leaf</tt>, etc.),
	belonging to MyEventClass, with class data members viewable in CINT by default, and created with the default
	constructor	<tt>MyEventClass()</tt>.

	rb::data::Wrapper provides "smart pointer"-like functionality; that is, you can get access to your class's members
	with the <tt>-></tt> and \c * operators, as well as the \c Get() (or <tt>get()</tt>) function:
	\code
	fMyClass->SomeMemberFunction(); // Call a member function of MyClass
	
	SomeFunction(*fMyClass); // SomeFunction takes a MyClass reference

	SomeOtherFunction(fMyClass.Get()); // SomeOtherFunction takes a MyClass pointer
	\endcode

	
	We've already made a bit of a mention of the concept of making class data members visible in CINT, as well as
	using comment fields to direct how certain data members should be processed into trees.  This subject likely
	needs a bit of expansion, which we'll do here.  First, we'll note that we assume that all class data members
	fall into one of three categories:
	-# Parameters - numbers which are calculated event-by-event from detector signals, and which you want to be
	able to further analyze (view in a histogram, set gates on, etc.)
	-# Variables - numbers which are used in the calculation of various parameters (e.g. calibration slopes and
	the like). Typically, you will want the ability to check, and possibly re-set, the values of parameters at
	any time during your analysis.
	-# Internal data - data used internally by your classes; this is something necessary in your analysis, but not
	something that you want to be able to view, either event-by-event like parameters or on demand like variables.

	You can control how specific data members of your classes are treated via the third argument of the
	rb::data::Wrapper constructor and by comment fields inserted after data members in your class definitions. As
	mentioned, the rb::data::Wrapper constructor argument decides whether or not to make class members available
	in CINT. In other words, setting this value to \c true will by default treat each data member as a variable,
	with access via the CINT command line (read access via rb::data::GetValue() and write access via
	rb::data::SetValue()) or via the <a href=gui.html#gui_variables>gui</a>.  Alternatively, setting the argument
	to \c false means that no data members in your class can be treated as variables.

	In addition to setting variable treatment in bulk as explained above, you can control the access levels of
	individual data members using comment fields in your class definitions.  The relevant comments should come
	after the data member is defined, still on the same line. Comments beginning with \c //! and \c //# are
	treated specially.  \c //! tells root to ignore the data member entirely when parsing the class, placing it into
	category (3) above.  \c //# tells root not to treat the data member as a variable (definition (2) above). Typically,
	you will want to use the \c //# comment whenever you have made your class full of variables by default (e.g. \c true
	rb::data::Wrapper 3rd constructor argument) but want specific data members to be treated as parameters. Hopefully, the
	simple example below will illustrate things better:
  \verbatim
  // Some user class, contains parameters, variables, and other data
  class Class1 {
  private:
    int internal; //! (Ignore this completely)
  public:
    double var; //  (No special comments, treat as a variable)
    double par; //# (Treat as a parameter)
  };
  	
  // Later, when invoking rb::data::Wrapper constructor
  // By default all data members are treated as variables, and the comment fields
  // noted above change the status of specific members.
  fClass1("class1", this, true, "")  \endverbatim
  
  We could also create a class that we don't want to contain any variables:
  \verbatim
  // Some user class, contains just parameters and other data
  class Class2 {
  private:
    int internal; //! (Ignore this completely)
  public:
    double par; // (Treat as a parameter, without any special comments)
  };
  	
  // Later, when invoking rb::data::Wrapper constructor
  // By default all data members are treated as parameters.
  fClass2("class2", this, false, "")	\endverbatim

  Once you have designed your event classes, what remains is to implement the virtual functions of rb::Event.
	Specifically, these are rb::Event::DoProcess(), where you tell how to unpack an event into your data classes,
	and rb::Event::HandleBadEvent(), where you tell what to do in case you receive a bad event in your data stream.


	\section data_buffer Adding a New Type of Buffer Source
	
	If your experiment does not use the NSCL or MIDAS data acquisition systems, then you will need to tell
	rootbeer how to handle your specific data format and how to connect to online or offline data sources.
	This is done in a similar way to event handling, namely you will have to create a class that derives from
	rb::BufferSource and then implement the appropriate virtual functions to tell rootbeer how to handle your
	data format.  There is a fair amount of documentation on the rb::BufferSource doxygen page, which should
	tell you what you need to know to write your derived class. Also, note that in addition to virtual functions,
	you will need to implement the static rb::BufferSource::New() function, such that it returns a \c new instance
	of your particular rb::BufferSource-derived class.

	If you have successfully designed and tested the implementation of a new buffer source class, please contact
	the rootbeer developers about having your implementation added to the main code base. This way, your collaborators
	won't have to spend time reproducing the work you've already done.

	\section data_orginization Code Orginization and Compilation

	Rootbeer is set up such that to modify for a given experiment, only four files will need to be edited. These are:
	-# src/user/User.hxx
	-# src/user/User.cxx
	-# user/Makefile.user
	-# user/UserLinkdef.h

	The first two files, User.hxx and User.cxx are where you can design and implement your rb::Event and rb::BufferSource
	derived classes. If you want to use source files from elsewhere, simply \c #include the appropriate headers (you can
	tell them to be compiled in Makefile.user as we'll explain).  Of course, if you prefer you can create new files to
	handle your class definitions/implementations, but to do this you will have to make some modifications to the
	Makefile directly (if you're wanting to do this, we'll assume you can figure out what to do).

	As mentioned, Makefile.user is where you tell rootbeer to compile your external sources, as well as where you
	can provide some other specific derectives for compilation.  The version of this file that ships with rootbeer
	contains comments telling you everything you will need to know to modify it appropriately.

	Finally, UserLinkdef.h lets you tell rootcint to generate a "dictionary" for your classes. This is required
	in order to take adantage of root's ability to parse your classes into a tree structure, as required by rootbeer.
	The doxygen page on UserLinkdef.h (or the comments in the file) should explain what you need to add to make things
	work.

	Once you have designed and implemented your rb::Event (and possible rb::BufferSource) derived classes, and made the
	necessary modifications to Makefile.user and UserLinkdef.h, you are ready to try compiling rootbeer, which simply
	requires a
	\code
  > make
	\endcode
	from the \c rootbeer directory. Successful compilation will generate the \c rootbeer executable in the \c rootbeer
	directory, as well as the \c libRootbeer.so shared library in the \c rootbeer/lib directory.	We hope that rootbeer
	will compile and run "out of the box" on your partucular platform (assuming everything is done properly on your end).
  If you run into troubles, please note the problems you're having, the solutions (if you find one) and contact the
	developers so that we can try to fix things.


  \page develop Developers

  \todo This page should contain some basic instructions for those wishing to contribute to rootbeer, including a
	bit about how to get started, desired features, coding standards, caveats, and so on.








*/
