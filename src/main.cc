//! \file main.cc
//! \brief Implements the \c main program and Doxygen documentation.
#include <set>
#include <TROOT.h>
#include "Rint.hxx"
#include "Rootbeer.hxx"
//#include "gui/CanvasGui.hxx"

/// \brief The \c main ROOTBEER function.
//! \details Creates an instance of \c rb::Rint and runs it.
Int_t main(Int_t argc, Char_t** argv)
{
	std::set<std::string> args(argv, argv+argc);
	Bool_t lite = args.count("-l");
  rb::Rint rbApp("Rootbeer", &argc, argv, 0, 0, lite);
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
	be a logical statement depending on relevant TTree parameters and constants. For a given event, each histogram is
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
	macro code to reproduce the directory structure and histograms present at save time (this is identical to the "Save"
	button on the histograms frame).  "Save Variables" has a similar effect, except is produces code to reproduce all
	of the variables and their values at save time. "Save Canvases" saves the canvas structure as
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
   create a "bare" instance of your class directly. Instead it creates a new global instance of rb::data::Wrapper <T>, with
   the template argument being your class type. Doing things this way automatically calls the appropriate functions
   allowing your data to be displayed in histograms, and furthermore it allows thread-safe access to the classes,
   both in compiled code and in CINT.  For more information, see the documentation on rb::data::Basic and rb::data::Wrapper.


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
   due to the issues that could arise in a threaded environment.  Instead, you'll need to use the GetPointer()
   \todo UPDATE "Acquiring and Unpacking Data" section
   of rb::data to obtain a special type of pointer that allows thread safe access to your data. This special pointer
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

   If you notice any difficulties that you think were the fault of the ROOTBEER design, or with this documentation,
   feel free to contact
   <a href = mailto:gchristian@triumf.ca>Greg</a> with comments, complaints, or suggestions.  As much as possible, we
   hope that adapting ROOTBEER to work with your experiment will be a painless experience, and we take suggestions on how
   to improve the ROOTBEER design and the interface at the user level very seriously.
   Of course, we also reserve the right to ignore you if we don't like what you have to say....

  \page develop Developers
  Developers....







*/
