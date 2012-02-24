//! \file Event.hxx
//! \brief Defines a base class for rootbeer event processors.
#ifndef EVENT_HXX
#define EVENT_HXX
#include <set>
#include <memory>
#include <string>
#include <sstream>
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
      void operator() (const Manager& histograms);
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
      HistContainer_t fSet;
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
  public:
    //! For histogram container management
    event::histogram::Manager Hists;
    //! For filling within Hists
    event::histogram::Filler FillAll;
  private:
    //! Mutex to protect access to critical elements.
    rb::Mutex fMutex;
    //! TTree associated with this event type.
    volatile std::auto_ptr<TTree> fTree;
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
    void Process();

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
    virtual Bool_t DoProcess() = 0;

    //! \brief Defines what should be done upon failure to successfully process an event.
    //! \details Users may want/need to handle bad events differently, e.g. by throwing an exception,
    //!  printing/logging an error message, aborting the program, etc. Since this is pure virtual, they get to choose.
    virtual void HandleBadEvent() = 0;
  };
} // namespace rb

#ifndef __MAKECINT__
#define SCOPED_LOCK rb::ScopedLock<rb::Mutex> lock(fMutex);
namespace {
  struct HistFill { Int_t operator() (rb::Hist* const& hist) {
    return hist->Fill();
  } } fill_hist;
}
inline void event::histogram::Filler::operator() (const Manager& histograms) {
  std::for_each(histograms.fSet.begin(), histograms.fSet.end(), fill_hist);
}
inline event::histogram::Manager::Manager(rb::Mutex& mutex) : fMutex(mutex) {
}
namespace {
  struct HistDelete { bool operator() (rb::Hist* const& hist) {
    delete hist;
  } } delete_hist;
}
inline event::histogram::Manager::~Manager() {
  SCOPED_LOCK;
  std::for_each(fSet.begin(), fSet.end(), delete_hist);
  fSet.clear();
}
inline void event::histogram::Manager::Add(rb::Hist* hist) {
  SCOPED_LOCK;
  fSet.insert(hist);
}
inline void event::histogram::Manager::Delete(rb::Hist* hist) {
  SCOPED_LOCK;
  delete hist;
  fSet.erase(hist);
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
inline void rb::Event::Process() {
  SCOPED_LOCK;
  Bool_t success = DoProcess();
  if(success) FillAll(Hists);
  else HandleBadEvent();
}
template <typename Derived> rb::Event*& rb::Event::Instance() {
  static Event* event = 0;
  if(!event) event = new Derived();
  return event;  
}
#undef SCOPED_LOCK
#endif

#endif
