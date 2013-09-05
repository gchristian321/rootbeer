//! \file Rootbeer.hxx
//! \brief Define the \c CINT user interface to <tt>ROOTBEER</tt>
//! \details The user interface to anything \c ROOTBEER specific is done via functions
//! in the \c rb namespace. So, to do any action specific to \c ROOTBEER (as opposed
//! to just plain <tt>ROOT</tt>), the user will call <tt>rb::DoWhatever()</tt>.  All
//! of the available user functions and their descriptions can be found in this Doxygen file.
#ifndef ROOTBEER_HXX
#define ROOTBEER_HXX
#include <Rtypes.h>

class TCutG;
class TDirectory;
class TVirtualPad;

/// Namespace wrapping the \c ROOTBEER objects and user functions.
namespace rb
{
/// \brief Attach to an online data sorce.
//! \details
//! \todo Implement for NSCL data.
void AttachOnline(const char* host, const char* other_arg = "", char** others = 0, int n_others = 0);

/// \brief Attach to an offline data source.
//! \param filename Path of the file to which you want to attach.
//! \param stop_at_end Specifies whether to Unattach() upon reaaching the
//! end of the file [true] or to stay attached and wait for more data [false].
void AttachFile(const char* filename, Bool_t stop_at_end = kTRUE);

/// \brief Attach to a series of offline data sources.
//! \param filename Path of a text file listing the files you want to attach to, one per line.
//! Blank lines and whitespace are ignored, as are lines beginning with <tt>#</tt>.
void AttachList(const char* filename);

/// \brief Disconnect from a data source.
//! Stops all reading of data and closes out the relevant threads.
void Unattach();

/// \brief Write canvas configuration file.
Int_t WriteCanvasXML(const char* filename, Bool_t prompt = kTRUE);

/// \brief Write histogram configuration file
Int_t WriteHistXML(const char* filename, Bool_t prompt = kTRUE);

/// \brief Write variables configuration file
Int_t WriteVariablesXML(const char* filename, Bool_t propmt = kTRUE);

/// \brief Write full configuration file (canvas, variable, hist)
Int_t WriteConfigXML(const char* filename, Bool_t prompt = kTRUE);

/// \brief Read histogram and directory information from an XML file
/// \param option See ReadConfigXML()
void ReadHistXML(const char* filename, Option_t* option = "r");

/// \brief Read canvas configuration file.
void ReadCanvasXML(const char* filename);

/// \brief Read variables configuration file
void ReadVariablesXML(const char* filename);

/// \brief Read complete configuration file
//! \param filename Path of the configuration file.
//! \param option String specifying how the configuration file should be read.
//! Options are:
//! -# "r" (reset). Delete all current histograms and gates and read in the new ones.
//! -# "o" (overwrite). Overwrite any matching gates or histograms, add any new ones (default).
//! -# "c" (cumulate). Delete nothing. Any duplicate names in the file will create a
//!     new histogram with \c _1, \c _2, etc. appended to the name.
//!
//!  Note that the option string is not case sensitive.
void ReadConfigXML(const char* filename, Option_t* option = "r");

/// Cd to a directory by its pathname
TVirtualPad* CdPad(TVirtualPad* owner, Int_t* subpad_numbers, Int_t depth);

/// Cd to a directory by its pathname
TDirectory* Cd(const char* path, Bool_t silent = false);

/// Create a new TDirectory as a sub-directory of the current one.
//! \note Also checks for duplicates and doesn't create the new directory if it already exists.
TDirectory* Mkdir(const char* name, const char* title = "");

/// Contains user functions relevant to updating canvases and other graphics.
namespace canvas
{
/// \brief Update every open canvas.
//! \details
void UpdateAll();

/// \brief Update the currently selected pad only.
//! \details
void UpdateCurrent();

/// \brief Clear all canvases.
//! \details
void ClearAll();

/// \brief Clear the current canvas.
//! \details
void ClearCurrent();

/// \brief Start updating canvases in a separate thread from the main one running CINT.
//! \details
//! \param rate How often the canvases should refresh (in seconds).
Int_t StartUpdate(Int_t rate = 5);

/// \brief Stop updating canvases.
//! \details
Int_t StopUpdate();

/// Return the canvas update rate.
//! \brief
Int_t GetUpdateRate();

} // namespace canvas

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
Double_t GetValue(const char* name);

/// Set the value of a user class data member.
//! \param [in] name The full name (TTree leaf) of the data member (see data::GetValue() for
//! an example).
//! \param [in] newvalue What you want to set the data keyed by <i>name</i> to.
void SetValue(const char* name, Double_t newvalue);


/// Print the fill name and current value of every data member in every listed class.
void PrintAll();

} // namespace data

/// Creation functions for histograms
namespace hist
{
class Base;

/// One-dimensional creation function
rb::hist::Base* New(const char* name, const char* title,
										Int_t nbinsx, Double_t xlow, Double_t xhigh,
										const char* param, const char* gate = "", Int_t event_code = 1);

/// Two-dimensional creation function
rb::hist::Base* New(const char* name, const char* title,
										Int_t nbinsx, Double_t xlow, Double_t xhigh,
										Int_t nbinsy, Double_t ylow, Double_t yhigh,
										const char* param, const char* gate = "", Int_t event_code = 1);

/// Three-dimensional creation function
rb::hist::Base* New(const char* name, const char* title,
										Int_t nbinsx, Double_t xlow, Double_t xhigh,
										Int_t nbinsy, Double_t ylow, Double_t yhigh,
										Int_t nbinsz, Double_t zlow, Double_t zhigh,
										const char* param, const char* gate = "", Int_t event_code = 1);

/// Summary histogram creation
rb::hist::Base* NewSummary(const char* name, const char* title,
													 Int_t nbins, Double_t low, Double_t high,
													 const char* paramList,  const char* gate = "", Int_t event_code = 1,
													 const char* orientation = "v");

/// Gamma hist creation (1d)
rb::hist::Base* NewGamma(const char* name, const char* title,
												 Int_t nbinsx, Double_t xlow, Double_t xhigh,
												 const char* params,  const char* gate = "", Int_t event_code = 1);

/// Gamma hist creation (2d)
rb::hist::Base* NewGamma(const char* name, const char* title,
												 Int_t nbinsx, Double_t xlow, Double_t xhigh,
												 Int_t nbinsy, Double_t ylow, Double_t yhigh,
												 const char* params,  const char* gate = "", Int_t event_code = 1);

/// Gamma hist creation (3d)
rb::hist::Base* NewGamma(const char* name, const char* title,
												 Int_t nbinsx, Double_t xlow, Double_t xhigh,
												 Int_t nbinsy, Double_t ylow, Double_t yhigh,
												 Int_t nbinsz, Double_t zlow, Double_t zhigh,
												 const char* params,  const char* gate = "", Int_t event_code = 1);

/// Scaler hist creation
rb::hist::Base* NewScaler(const char* name, const char* title,
													Int_t nbins, Double_t low, Double_t high,
													const char* params, const char* gate = "", Int_t event_code = 1);
																 

/// Bit hist creation
rb::hist::Base* NewBit (const char* name, const char* title, Int_t nbits, const char* param,
												const char* gate = "", Int_t event_code = 1);

/// Zero all histograms
void ClearAll();

} // namespace hist

} // namespace rb

#endif
