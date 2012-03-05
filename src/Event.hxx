//! \file Event.hxx
//! \brief Defines a base class for rootbeer event processors.
#ifndef EVENT_HXX
#define EVENT_HXX
#include <set>
#include <memory>
#include <string>
#include <sstream>
#include <iostream>
#include <typeinfo>
#include <algorithm>
#include <TClass.h>
#include <TTree.h>
#include <TTreeFormula.h>
#include "hist/Manager.hxx"
#include "utils/LockingPointer.hxx"
#include "utils/Error.hxx"
#include "utils/nocopy.h"
#ifndef __MAKECINT__
#include "boost/scoped_ptr.hpp"
#else
namespace boost { template <class T> class scoped_ptr; }
#endif


namespace rb
{
  typedef boost::scoped_ptr<volatile TTreeFormula> FormulaPtr_t;

  class Rint;
  class TreeFormulae;
  namespace data { template <class T> class Wrapper; }

  /// \brief Abstract base class for event processors.
  class Event
  {
    RB_NOCOPY(Event);
    private:
    boost::scoped_ptr<volatile TTree> fTree;

    //! Manages histograms associated with the event
    hist::Manager fHistManager;

  public:
    //! Return a pointer to fHistManager
    hist::Manager* const GetHistManager();

    //! \brief Public interface for processing an event.
    //! \details The real work for actually doing something with the event data
    //! is done in the virtual member DoProcess(). This function just takes care
    //! of behind-the-scenes stuff like filling histograms and mutex locking.
    //! \param addr Address of the beginning of the event.
    //! \param [in] nchar length of the event in bytes.
    void Process(void* event_address, Int_t nchar);

    //! \brief Singleton instance function.
    //! \details Each derived class is a singleton, with only one instance allowed.
    //!  Use this function to get a pointer to the single instance of derived class <i>Derived</i>.
    //! \tparam Derived The type of the derived class you want an instance of.
    //! \returns Pointer to the single instance of <i>Derived</i>
    template <typename Derived> static Event*& Instance();

  protected:
    //! Initialize data members
    Event();
    //! Nothing to do in the base class, all data is self-destructing.
    virtual ~Event();

  private:
    //! \brief Does the work of processing an event.
    //! \details Users shold implement this in derived classes to instruct the program
    //!  on how to unpack event data into their classes.
    //! \returns Error code: true upon successful unpack, false otherwise.
    virtual Bool_t DoProcess(void* event_address, Int_t nchar) = 0;

    //! \brief Defines what should be done upon failure to successfully process an event.
    //! \details Users may want/need to handle bad events differently, e.g. by throwing an exception,
    //!  printing/logging an error message, aborting the program, etc. Since this is pure virtual, they get to choose.
    virtual void HandleBadEvent() = 0;

  public:
    /// Adds a branch to the event tree.
    class BranchAdd
    {
      /// Perform the branch adding
      //! \returns true upon successful branch creation, false otherwise
      static Bool_t Operate(rb::Event* const event, const char* name, const char* classname, void** address, Int_t bufsize);
    
      /// Give access to rb::data::Wrapper
      template <class T> friend class rb::data::Wrapper;
    };

    /// Deletes an event and sets the pointer to NULL.
    class Destructor
    {
      /// Perform the destruction
      static void Operate(rb::Event*& event);

      /// Give access to rb::Rint (called from Terminate())
      friend class rb::Rint;
      friend class Event;
    };

    /// Initialize a TTreeFormula
    class InitFormula
    {
      /// Perform the initialization
      //! \param [in] formula_arg String specifying the formula argument
      //! \param [out] ttf The smart pointer to the TTreeFormula you want to initialize
      static Bool_t Operate(rb::Event* const event, const char* formula_arg, FormulaPtr_t& ttf);

      /// Give access to rb::TreeFormulae
      friend class rb::TreeFormulae;
    };

#ifndef __MAKECINT__
    friend void Destructor::Operate(Event*&);
    friend Bool_t InitFormula::Operate(Event* const, const char*, FormulaPtr_t&);
    friend Bool_t BranchAdd::Operate(Event* const, const char*, const char*, void**, Int_t);
#endif
  };
} // namespace rb



#ifndef __MAKECINT__

// ======== Inlined Function Implementations ========= //

inline rb::Event::~Event() {
}

inline rb::hist::Manager* const rb::Event::GetHistManager() {
  return &fHistManager;
}

inline void rb::Event::Destructor::Operate(rb::Event*& event) {
  delete event;
  event = 0;
}

template <typename Derived> rb::Event*& rb::Event::Instance() {
  static Event* event = 0;
  if(!event) event = new Derived();
  return event;  
}

#endif

#endif
