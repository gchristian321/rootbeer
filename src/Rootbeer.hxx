//! \file Rootbeer.hxx
//! \brief Define the \c CINT user interface to <tt>ROOTBEER</tt>
//! \details The user interface to anything \c ROOTBEER specific is done via functions
//! in the \c rb namespace. So, to do any action specific to \c ROOTBEER (as opposed
//! to just plain <tt>ROOT</tt>), the user will call <tt>rb::DoWhatever()</tt>.  All
//! of the available user functions and their descriptions can be found in this Doxygen file.
#ifndef ROOTBEER_HXX
#define ROOTBEER_HXX
#include <Rtypes.h>

/// Namespace wrapping the \c ROOTBEER objects and user functions.
namespace rb
{
  /// \brief Attach to an online data sorce.
  //! \details
  //! \todo Implement for NSCL data.
  extern void AttachOnline(const char* host, const char* other_arg = "", char** others = 0, int n_others = 0);

  /// \brief Attach to an offline data source.
  //! \param filename Path of the file to which you want to attach.
  //! \param stop_at_end Specifies whether to Unattach() upon reaaching the
  //! end of the file [true] or to stay attached and wait for more data [false].
  extern void AttachFile(const char* filename, Bool_t stop_at_end = kTRUE);

  /// \brief Attach to a series of offline data sources.
  //! \param filename Path of a text file listing the files you want to attach to, one per line.
  //! Blank lines and whitespace are ignored, as are lines beginning with <tt>#</tt>.
  extern void AttachList(const char* filename);

  /// \brief Disconnect from a data source.
  //! Stops all reading of data and closes out the relevant threads.
  extern void Unattach();

  /// \brief Write configuration file
  //! \details The configureation file is CINT code that reproduces things you've defined in ROOTBEER
  //! so far: histogram definitions, directories, and variable values.
  //! \param filename Path of the configuration file you're writing.
  //! \param prompt Boolean: true = ask to overwrite existing files, false = overwrite automatically.
  extern Int_t WriteConfig(const char* filename, Bool_t prompt = kTRUE);

  /// \brief Write configuration file (directories and histograms only).
  //! \details Same as WriteConfig(), except only histograms and directories are written.
  extern Int_t WriteHistograms(const char* filename, Bool_t prompt = kTRUE);

  /// \brief Write configuration file (variables only).
  //! \details Same as WriteConfig(), except only variables and their values are written.
  extern Int_t WriteVariables(const char* filename, Bool_t prompt = kTRUE);

  /// \brief Read a configuration file.
  //! \param filename Path of the configuration file.
  //! \parma option String specifying how the configuration file should be read.
  //! Options are:
  //! -# "r" (reset). Delete all current histograms and gates and read in the new ones.
  //! -# "o" (overwrite). Overwrite any matching gates or histograms, add any new ones (default).
  //! -# "c" (cumulate). Delete nothing. Any duplicate names in the file will create a
  //!     new histogram with \c _1, \c _2, etc. appended to the name.
  //!
  //!  Note that the option string is not case sensitive.
  extern void ReadConfig(const char* filename, Option_t* option = "o");

  /// \brief Write canvas configuration file.
  //! \details Writes the current canvases and the histograms contained on them to a configuration
  //! file.
  //! \todo Currently not implemented.
  extern Int_t WriteCanvases(const char* filename, Bool_t prompt = kTRUE);

  /// Contains user functions relevant to updating canvases and other graphics.
  namespace canvas
  {
    /// \brief Update every open canvas.
    //! \details
    extern void UpdateAll();

    /// \brief Update the currently selected pad only.
    //! \details
    extern void UpdateCurrent();

    /// \brief Clear all canvases.
    //! \details
    extern void ClearAll();

    /// \brief Clear the current canvas.
    //! \details
    extern void ClearCurrent();

    /// \brief Start updating canvases in a separate thread from the main one running CINT.
    //! \details
    //! \param rate How often the canvases should refresh (in seconds).
    extern Int_t StartUpdate(Int_t rate = 5);

    /// \brief Stop updating canvases.
    //! \details
    extern Int_t StopUpdate();

    /// Return the canvas update rate.
    //! \brief
    extern Int_t GetUpdateRate();

  }
  /// Wraps classes and functions relevant to the access of data and methods of user classes.
  namespace data
  {
    /// Get the value of a user class data member.
    //! \param [in] name The full name (TTree leaf) of the data member. For example:
    //! \code
    //! struct AStruct {
    //!   int a;
    //! } mine;
    //! \endcode
    //! would be accessed by
    //! \code
    //! rb::data::GetValue("mine.a");
    //! \endcode
    //! \returns The value of the basic data member keyed by <i>name</i>, casted
    //! to a Double_t.
    extern Double_t GetValue(const char* name);

    /// Set the value of a user class data member.
    //! \param [in] name The full name (TTree leaf) of the data member (see data::GetValue() for
    //! an example).
    //! \param [in] newvalue What you want to set the data keyed by <i>name</i> to.
    extern void SetValue(const char* name, Double_t newvalue);


    /// Print the fill name and current value of every data member in every listed class.
    extern void PrintAll();
  }
  /// Creation functions for histograms
  namespace hist
  {
    class Base;

    /// One-dimensional creation function
    extern rb::hist::Base* New(const char* name, const char* title,
			       Int_t nbinsx, Double_t xlow, Double_t xhigh,
			       const char* param, const char* gate = "", Int_t event_code = 1);

    /// Two-dimensional creation function
    extern rb::hist::Base* New(const char* name, const char* title,
			       Int_t nbinsx, Double_t xlow, Double_t xhigh,
			       Int_t nbinsy, Double_t ylow, Double_t yhigh,
			       const char* param, const char* gate = "", Int_t event_code = 1);

    /// Three-dimensional creation function
    extern rb::hist::Base* New(const char* name, const char* title,
			       Int_t nbinsx, Double_t xlow, Double_t xhigh,
			       Int_t nbinsy, Double_t ylow, Double_t yhigh,
			       Int_t nbinsz, Double_t zlow, Double_t zhigh,
			       const char* param, const char* gate = "", Int_t event_code = 1);

    /// Summary histogram creation
    extern rb::hist::Base* NewSummary(const char* name, const char* title,
				      Int_t nbins, Double_t low, Double_t high,
				      const char* paramList,  const char* gate = "", Int_t event_code = 1,
				      const char* orientation = "v");

    /// Gamma hist creation (1d)
    extern rb::hist::Base* NewGamma(const char* name, const char* title,
				    Int_t nbinsx, Double_t xlow, Double_t xhigh,
				    const char* params,  const char* gate = "", Int_t event_code = 1);

    /// Gamma hist creation (2d)
    extern rb::hist::Base* NewGamma(const char* name, const char* title,
				    Int_t nbinsx, Double_t xlow, Double_t xhigh,
				    Int_t nbinsy, Double_t ylow, Double_t yhigh,
				    const char* params,  const char* gate = "", Int_t event_code = 1);

    /// Gamma hist creation (3d)
    extern rb::hist::Base* NewGamma(const char* name, const char* title,
				    Int_t nbinsx, Double_t xlow, Double_t xhigh,
				    Int_t nbinsy, Double_t ylow, Double_t yhigh,
				    Int_t nbinsz, Double_t zlow, Double_t zhigh,
				    const char* params,  const char* gate = "", Int_t event_code = 1);

    /// Bit hist creation
    rb::hist::Base* NewBit (const char* name, const char* title, Int_t nbits, const char* param,
			    const char* gate = "", Int_t event_code = 1);

  }
}

#endif
