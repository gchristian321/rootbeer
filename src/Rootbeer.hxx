/*! \file rootbeer.hxx
 * \brief Define the \c CINT user interface to <tt>ROOTBEER</tt>
 *
 * The user interface to anyything \c ROOTBEER specific is done via functions
 * in the \c rb namespace. So, to do any action specific to \c ROOTBEER (as opposed
 * to just plain <tt>ROOT</tt>), the user will call <tt>rb::DoWhatever()</tt>.  All
 * of the available user functions and their descriptions can be found in this Doxygen
 * file.
 *
 * \note \c CINT users have no access to the variables defined here since they're not
 * included in the \c Linkdef.h file. This is done on purpose to keep users from inadvertently
 * doing something they shouldn't to a variable and causing a crash or other bad behavior.
 */



#ifndef __ROOTBEER__
#define __ROOTBEER__

#include "TObjArray.h"
#include "Unpacker.hxx"


/// \global Global array of all \c rb::HnF objects.
extern TObjArray gHistograms;

/// \global Global unpacker class instance.
extern rb::Unpacker gUnpacker;


/// Namespace wrapping the \c ROOTBEER objects and user functions.
namespace rb
{
  /// Attach to an online data sorce.
  extern void AttachOnline();

  /// Disconnect from a data source.
  extern void Unattach();

  /// Function to create a 1d histogram. Mirors the \c TH1D constructor.
  extern void AddHist(const char* name, const char* title,
		      Int_t nbinsx, Double_t xlow, Double_t xhigh,
		      const char* param, const char* gate = "");

  /// Function to create a 2d histogram. Mirors the \c TH2D constructor.
  extern void AddHist(const char* name, const char* title,
		      Int_t nbinsx, Double_t xlow, Double_t xhigh,
		      Int_t nbinsy, Double_t ylow, Double_t yhigh,
		      const char* param, const char* gate = "");

  /// Function to create a 3d histogram. Mirors the \c TH3D constructor.
  extern void AddHist(const char* name, const char* title,
		      Int_t nbinsx, Double_t xlow, Double_t xhigh,
		      Int_t nbinsy, Double_t ylow, Double_t yhigh,
		      Int_t nbinsz, Double_t zlow, Double_t zhigh,
		      const char* param, const char* gate = "");

  /// Return a \c const pointer to the \c gHistograms list.
  extern const TObjArray* GetListOfHistograms();

  /// Display the \c ROOTBEER logo.
  extern void Logo();

  /// Contains user functions relevant to updating canvases and other graphics.
  namespace canvas
  {
    /// Update every open canvas.
    extern void UpdateAll();

    /// Update the currently selected pad only.
    extern void UpdateCurrent();

    /// Start updating canvases in a separate thread from the main one running \c CINT.
    extern Int_t StartUpdate(Int_t rate);

    /// Stop updating canvases.
    extern Int_t StopUpdate();

    /// Return the canvas update rate.
    extern Int_t GetUpdateRate();
  }

}



#endif
