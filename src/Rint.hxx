#ifndef RINT_HXX
#define RINT_HXX
#include <map>
#include <vector>
#include <sstream>
#include <TRint.h>
#include "Event.hxx"
#include "Signals.hxx"
#include "utils/Error.hxx"


class TDirectory;

namespace rb
{
//========== Typedefs ===========//
typedef std::map<Int_t, std::pair<rb::Event*, std::string> > EventMap_t;
typedef std::vector<std::pair<Int_t, std::string> > EventVector_t;

//========== Forward Declarations ===========//
namespace gui { class MainFrameFactory; class MainFrame; }
namespace hist { class Base; }

//========== Class Definitions ===========//
/// \brief Class that runs the interactive ROOT application.
//! \details We can essentially use the normal <tt>TRint</tt>, except
//!  we need to override the Terminate() method to stop threaded processes.
class Rint: public TRint
{
private:
	 //! Messages to be printed at program startup
	 std::stringstream fMessage;
	 //! Map of all event processors, keyed by an integer code.
	 EventMap_t fEvents;
	 //! Manages construction & destruction of gui objects
	 gui::MainFrameFactory* fFrames;
	 //! Signal emitter
	 Signals fSignals;
	 //! Tells whether or not to save tree data to disk.
	 Bool_t fSaveData;
	 //! Tells whether or not to save histograms to disk.
	 //! \note can only be true if fSaveData is also
	 Bool_t fSaveHists;
public:
   //! Call Sync() on all gui frames
	 void SyncAll();

   //! Returns a pointer to fSignals
	 rb::Signals* GetSignals();

	 //! Find the event processors with a specific code.
	 //! \param [in] code Event code you're searching for
	 //! \returns a pointer to the event with code <i>code</i>. In the case of invalid code,
	 //!  return NULL
	 rb::Event* GetEvent(Int_t code);

   //! Return a vector populated with pairs of < event code, event name >
	 EventVector_t GetEventVector();

	 //! Return the number of events total
	 Int_t NEvents() { return fEvents.size(); }

	 //! Turn on saving of data
	 //! \param [in] save_hists true means to save historams also
	 void StartSave(Bool_t save_hists);
	 
	 //! Turn off saving of data
	 void StopSave();

	 //! Returns fSaveData
	 Bool_t GetSaveData();
	 
	 //! Returns fSaveHists
	 Bool_t GetSaveHists();

	 //! Search for a histogram by name
   //! \param [in] name Histogram name
	 //! \param [in] Directory owing the histogram, passing 0 searches globally and simply returns the first result.
	 rb::hist::Base* FindHistogram(const char* name, TDirectory* owner = 0);

	 /// \brief Constructor
	 //! \details Just call the standard \c TRint constructor, plus set the prompt to be
	 //! <tt>rootbeer [\%d]</tt>.
	 //! \note The \c \%d means that the number of commands entered in the session is
	 //! what's present.
	 Rint(const char* appClassName, int* argc, char** argv,
				void* options = 0, int numOptions = 0, Bool_t noLogo = kFALSE);
	 /// \brief Terminate the application.
	 //! \details Stops any running threads and frees any memory that was allocated during
	 //! the CINT session.
	 void Terminate(Int_t status = 0);
	 /// \brief Display the \c ROOTBEER logo.
	 //! \details ASCII art of naturally clumsy man stumbling whilst carrying two full mugs of root beer.
	 //! \param [in] lite 'false' prints the full logo, 'true' just prints a welcome message.
	 virtual void PrintLogo(Bool_t lite);
	 /// Add something to print at startup.
	 void AddMessage(const std::string& str);
	 /// \brief Destructor
	 //! \details Calls Terminate() with error code.
	 ~Rint();

private:
	 /// Adds an instance of an event processor to fEvents.
	 //! \tparam T The type of the instance you want to register.
	 template <typename T> void RegisterEvent(Int_t code, const char* name);

	 /// \brief Registers all event processors in the program.
	 //! \details This needs to be implemented by users to account for
	 //! the different event processors they want to use in the program.
	 //! See rb::Event documentation for more information.
	 void RegisterEvents();

   /// \brief Adds gui objects to fFrames
	 void InitGui();
public:
	 ClassDef(rb::Rint, 0);
}; // class Rint


//========== Globals ===========//
namespace
{ // Cast of ROOT's global gApplication pointer into an rb::Rint
inline Rint* gApp() { return static_cast<rb::Rint*>(gApplication); }
}
} // namesapce rb


  //========== Inlined Functions ===========//
#ifndef __MAKECINT__
inline rb::Rint::~Rint() {
  Terminate(1);
}
inline void rb::Rint::AddMessage(const std::string& str) {
  fMessage << str;
}
template <typename T>
void rb::Rint::RegisterEvent(Int_t code, const char* name) {
	if(code == -1) {
		err::Error("RegisterEvent")
			 << "Event code -1 is reserved and cannot be set by users.\n"
			 << "Please choose another code for " << name << "\n";
		exit(1);
	}
	std::pair<Event*, std::string> second = std::make_pair(rb::Event::Instance<T>(),
																												 std::string(name));
  fEvents.insert(std::make_pair(code, second));
}
inline rb::Signals* rb::Rint::GetSignals() {
	return &fSignals;
}
inline void rb::Rint::StartSave(Bool_t save_hists) {
	fSaveData = true;
	fSaveHists = save_hists;
}
inline void rb::Rint::StopSave() {
	fSaveData = false;
	fSaveHists = false;
}
inline Bool_t rb::Rint::GetSaveHists() {
	return fSaveHists;
}
inline Bool_t rb::Rint::GetSaveData() {
	return fSaveData;
}

#endif

#endif
