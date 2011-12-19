/*! \file rootbeer.hxx
 * \brief Define the \c CINT user interface to <tt>ROOTBEER</tt>
 *
 * The user interface to anything \c ROOTBEER specific is done via functions
 * in the \c rb namespace. So, to do any action specific to \c ROOTBEER (as opposed
 * to just plain <tt>ROOT</tt>), the user will call <tt>rb::DoWhatever()</tt>.  All
 * of the available user functions and their descriptions can be found in this Doxygen
 * file.
 */
#ifndef __ROOTBEER__
#define __ROOTBEER__

#include <TCutG.h>
#include <TRint.h>
#include "Hist.hxx"



/// Namespace wrapping the \c ROOTBEER objects and user functions.
namespace rb
{
  /// Attach to an online data sorce. Implemented in Unpack.cxx
  //! \todo Implement for real data.
  extern void AttachOnline();

  /// Attach to an offline data source. Implemented in Unpack.cxx
  extern void AttachFile(const char* filename, Bool_t stop_at_end = kTRUE);

  /// Attach to a series of offline data sources. Implemented in Unpack.cxx
  //! The argument is a text file listing the files you want to attach to, one per line.
  //! Blank lines and whitespace are ignored, as are lines beginning with <tt>#</tt>.
  extern void AttachList(const char* filename);

  /// Disconnect from a data source. Implemented in Unpack.cxx
  extern void Unattach();

  /// Display the \c ROOTBEER logo. Implemented in Rootbeer.cxx
  extern void Logo();

  /// Write configuration file
  extern Int_t WriteConfig(const char* filename, Bool_t prompt = kTRUE);

  /// Write configuration file (directories and histograms only).
  extern Int_t WriteHistograms(const char* filename, Bool_t prompt = kTRUE);

  /// Write configuration file (variables only).
  extern Int_t WriteVariables(const char* filename, Bool_t prompt = kTRUE);

  /// Read a configuration file.
  //! Options are:
  //! -# "r" (reset). Delete all current histograms and gates and read in the new ones.
  //! -# "o" (overwrite). Overwrite any matching gates or histograms, add any new ones (default).
  //! -# "c" (cumulate). Delete nothing. Any duplicate names in the file will create a
  //!     new histogram with \c _1, \c _2, etc. appended to the name.
  //!
  //!  Note that the option string is not case sensitive.
  extern void ReadConfig(const char* filename, Option_t* option = "o");

  /// Write canvas configuration file.
  extern Int_t WriteCanvases(const char* filename, Bool_t prompt = kTRUE);

  /// Contains user functions relevant to updating canvases and other graphics.
  namespace Canvas
  {
    /// Update every open canvas. Implemented in Canvas.cxx
    extern void UpdateAll();

    /// Update the currently selected pad only. Implemented in Canvas.cxx
    extern void UpdateCurrent();

    /// Clear all canvases. Implemented in Canvas.cxx
    extern void ClearAll();

    /// Clear the current canvas. Implemented in Canvas.cxx
    extern void ClearCurrent();

    /// \brief Start updating canvases in a separate thread from the main one running CINT.
    /// Implemented in Canvas.cxx
    extern Int_t StartUpdate(Int_t rate = 5);

    /// Stop updating canvases. Implemented in Canvas.cxx
    extern Int_t StopUpdate();

    /// Return the canvas update rate. Implemented in Canvas.cxx
    extern Int_t GetUpdateRate();

  }

  namespace CutG {
    extern TCutG* New(const char* name, const char* title, const char* varx, const char* vary,
		      const std::vector<Double_t>& xpoints, const std::vector<Double_t>& ypoints,
		      Color_t fillColor = kWhite, Color_t LineColor = kBlack, Int_t lineWidth = 2, Bool_t overwrite = kFALSE);
  }
    
  /// Class that runs the interactive ROOT application.
  /*! We can essentially use the normal <tt>TRint</tt>, except
   *  we need to override the Terminate()
   *  method to stop threaded processes. */
  class Rint : public TRint
  {
  public:
    /// Constructor
    //! Just call the standard \c TRint constructor, plus set the prompt to be
    //! <tt>rootbeer [\%d]</tt>.
    //! \note The \c \%d means that the number of commands entered in the session is
    //! what's present.
    Rint(const char* appClassName, int* argc, char** argv,
	 void* options = 0, int numOptions = 0, Bool_t noLogo = kFALSE) :
      TRint(appClassName, argc, argv, options, numOptions, noLogo) {
      SetPrompt("rootbeer [%d] ");
    }

    /// Terminate the application.
    //! Stops any running threads and frees any memory that was allocated during
    //! the CINT session.
    void Terminate(Int_t status = 0);

    /// Destructor
    //! Calls Terminate() with error code.
    ~Rint() {
      Terminate(EXIT_FAILURE);
    }
  };


}



#endif
