#ifndef RINT_HXX
#define RINT_HXX
#include <map>
#include <sstream>
#include <TRint.h>
#include "Event.hxx"
#include "Globals.hxx"
#include "rb_import_data.h"


typedef std::multimap<Int_t, rb::Event*> EventMap_t;
typedef std::pair<EventMap_t::iterator, EventMap_t::iterator> EventRange_t;
typedef std::vector<rb::Event*> Events_t;

namespace rb
{
  /// \brief Class that runs the interactive ROOT application.
  //! \details We can essentially use the normal <tt>TRint</tt>, except
  //!  we need to override the Terminate() method to stop threaded processes.
  class Rint : public TRint
  {
  private:
    //! Messages to be printed at program startup
    std::stringstream fMessage;

    //! Map of all event processors, keyed by an integer code.
    EventMap_t fEvents;
    
  public:
    //! Find all event processors with a specific code.
    //! \param [in] code Event code you're searching for
    //! \returns a vector containing a pointer to each event with code <i>code</i>.
    Events_t GetEvents(Int_t code);

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
    template <typename T>
    void RegisterEvent(Int_t code) {
      fEvents.insert(std::make_pair<Int_t, rb::Event*>(code, rb::Event::Instance<T>()));
    }

    /// \brief Registers all event processort in the program.
    //! \details This needs to be implemented by users to account for
    //! the different event processors they want to use in the program.
    //! See rb::Event documentation for more information.
    void RegisterEvents();

  public:
    //! Encapsulates the global rb::data::Wrapper <T> objects.
    globals::Data fDataGlobals;

    ClassDef(rb::Rint, 0);
  };

  namespace
  { // Cast of ROOT's global gApplication pointer into an rb::Rint
    inline Rint* gApp() { return static_cast<rb::Rint*>(gApplication); }
  }
  namespace globals
  {
    namespace
    {
      inline Data* GetData() { return &(gApp()->fDataGlobals); }

      inline rb::Mutex& Mutex() { return static_cast<rb::Rint*>(gApplication)->fDataGlobals.GetMutex(); }
    }
  }
} // namespace rb
inline rb::Rint::~Rint() {
  Terminate(1);
}
inline void rb::Rint::AddMessage(const std::string& str) {
  fMessage << str;
}

#endif
