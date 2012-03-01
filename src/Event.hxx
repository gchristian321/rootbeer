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

namespace rb { class Event; }
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
    volatile std::auto_ptr<TTree> fTree;

    //! For histogram container management
    hist::Manager fHistManager;
  protected:
    //! Initialize data members
    Event();
    //! Nothing to do in the base class, all data is self-destructing.
    virtual ~Event();
 
 public:
    rb::Mutex* GetMutex();
#ifndef __MAKECINT__
    //! \brief Get a locked pointer to fTree
    CountedLockingPointer<TTree> GetTree();
#endif
    TTree* GetTreeUnlocked();

    hist::Manager* const GetHistManager() { return &fHistManager; }

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
    return hist->FillUnlocked();
  } } fill_hist;
}
inline rb::Event::Event() :
  fMutex(false),
  fTree(new TTree("tree", "Rootbeer event tree")),
  fHistManager(GetTreeUnlocked(), fMutex) {
  GetTree()->SetDirectory(0);
}
inline rb::Event::~Event() {
}
inline void rb::Event::Destruct(Event*& event) {
  delete event;
  event = 0;
}
inline rb::Mutex* rb::Event::GetMutex() {
  return &fMutex;
}
inline CountedLockingPointer<TTree> rb::Event::GetTree() {
  volatile TTree* tree = LockFreePointer<std::auto_ptr<TTree> >(fTree)->get();
  CountedLockingPointer<TTree> p_tree(tree, &fMutex);
  return p_tree;
}
inline TTree* rb::Event::GetTreeUnlocked() {
  TTree* tree = LockFreePointer<std::auto_ptr<TTree> >(fTree)->get();
  return tree;
}
inline void rb::Event::Process(void* event_address, Int_t nchar) {
  ScopedLock<rb::Mutex> lock(fMutex);
  Bool_t success = DoProcess(event_address, nchar);
  if(success) fHistManager.FillAll();
  else HandleBadEvent();
}
template <typename Derived> rb::Event*& rb::Event::Instance() {
  static Event* event = 0;
  if(!event) event = new Derived();
  return event;  
}
#endif

#endif
