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

#include "Hist.hxx"



/// Namespace wrapping the \c ROOTBEER objects and user functions.
namespace rb
{
  /// Attach to an online data sorce. Implemented in Unpack.cxx
  extern void AttachOnline();

  /// Attach to an offline data source. Implemented in Unpack.cxx
  /*! \todo Allow this to read till end, then pick up where it left off.*/
  extern void AttachFile(const char* filename);

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
  extern void ReadConfig(const char* filename);

  /// Write canvas configuration file.
  extern Int_t WriteCanvases(const char* filename, Bool_t prompt = kTRUE);

  /// Contains user functions relevant to updating canvases and other graphics.
  namespace canvas
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
    extern Int_t StartUpdate(Int_t rate);

    /// Stop updating canvases. Implemented in Canvas.cxx
    extern Int_t StopUpdate();

    /// Return the canvas update rate. Implemented in Canvas.cxx
    extern Int_t GetUpdateRate();

  }

}



#endif
