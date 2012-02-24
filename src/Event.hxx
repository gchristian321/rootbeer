//! \file Event.hxx
//! \brief Defines a base class for rootbeer event processors.
#ifndef EVENT_HXX
#define EVENT_HXX
#include <set>
#include <memory>
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>
#include "Hist.hxx"
#include "utils/Mutex.hxx"

typedef std::set<rb::Hist*> HistContainer_t;
namespace rb { class Event; }
//! Helper classes
namespace event
{
  //! Helper classes related to histograms
  namespace histogram
  {
    /// \brief Functor class for filling histograms.
    //! \details Filling all histograms through the private operator() and then
    //! friending Event ensures that FillAll() can only be called from Event but without
    //! making fSet accessable except to Event::Histograms.
    class Manager;
    class Filler
    {
    private:
      //! Call Fill() on every entry in <i>histograms</i>.
      void operator() (Manager& manager);
    public:
      //! Give access to operator()
      friend class rb::Event;
    };
    /// \brief Wraps a container of all histograms registered to this event type.
    //! \details Also takes care of functions for Adding and Deleting histograms.
    class Manager
    {
    private:
      //! Container of pointers to histograms registered to this event type.
      volatile HistContainer_t fSet;
      //! Reference to Event::fMutex
      rb::Mutex& fMutex;
    public:
      //! Set fMutex
      Manager(rb::Mutex& fMutex);
      //! Deletes all entries in fSet
      ~Manager();
      //! Add a histogram to fSet
      void Add(rb::Hist* hist);
      //! Delete a histogram and remove from fSet
      void Delete(rb::Hist* hist);
      //! Give access to fSet
      friend class Filler;
    };
  }
}
namespace rb
{
  /// \brief Abstract base class for event processors.
  class Event
  {
  private:
    //! Mutex to protect access to critical elements.
    //! \warning This should always be the first data member declared since it is used in the destructor
    //! of other members.
    rb::Mutex fMutex;
    //! TTree associated with this event type.
    volatile std::auto_ptr<TTree> fTree;
  public:
    //! For filling within Hists
    event::histogram::Filler FillAll;
    //! For histogram container management
    event::histogram::Manager Hists;
  protected:
    //! Initialize data members
    Event();
    //! Nothing to do in the base class, all data is self-destructing.
    virtual ~Event();
  public:
#ifndef __MAKECINT__
    //! \brief Get a locked pointer to fTree
    CountedLockingPointer<TTree> GetTree();
#endif
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

    //! Call the destructor on an instance of Event and set the pointer to zero. 
    static void Destruct(Event*& event);

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
  };
} // namespace rb

#ifndef __MAKECINT__
namespace {
  struct HistFill { Int_t operator() (rb::Hist* const& hist) {
    return hist->Fill();
  } } fill_hist;
}
inline void event::histogram::Filler::operator() (Manager& manager) {
  LockFreePointer<HistContainer_t> pSet(manager.fSet);
  std::for_each(pSet->begin(), pSet->end(), fill_hist);
}
inline event::histogram::Manager::Manager(rb::Mutex& mutex) : fMutex(mutex) {
}
namespace {
  struct HistDelete { bool operator() (rb::Hist* const& hist) {
    delete hist;
  } } delete_hist;
}
inline event::histogram::Manager::~Manager() {
  LockingPointer<HistContainer_t> pSet(fSet, fMutex);
  std::for_each(pSet->begin(), pSet->end(), delete_hist);
  pSet->clear();
}
inline void event::histogram::Manager::Add(rb::Hist* hist) {
  LockingPointer<HistContainer_t> pSet(fSet, fMutex);;
  pSet->insert(hist);
}
inline void event::histogram::Manager::Delete(rb::Hist* hist) {
  LockingPointer<HistContainer_t> pSet(fSet, fMutex);;
  delete hist;
  pSet->erase(hist);
}
inline rb::Event::Event() :
  fMutex(false), Hists(fMutex),
  fTree(new TTree("tree", "Rootbeer event tree")) {
}
inline rb::Event::~Event() {
}
inline void rb::Event::Destruct(Event*& event) {
  delete event;
  event = 0;
}
inline CountedLockingPointer<TTree> rb::Event::GetTree() {
  volatile TTree* tree = LockFreePointer<std::auto_ptr<TTree> >(fTree)->get();
  CountedLockingPointer<TTree> pTree(tree, &fMutex);
  return pTree;
}
inline void rb::Event::Process(void* event_address, Int_t nchar) {
  ScopedLock<rb::Mutex> lock(fMutex);
  Bool_t success = DoProcess(event_address, nchar);
  if(success) FillAll(Hists);
  else HandleBadEvent();
}
template <typename Derived> rb::Event*& rb::Event::Instance() {
  static Event* event = 0;
  if(!event) event = new Derived();
  return event;  
}
#endif

#endif
