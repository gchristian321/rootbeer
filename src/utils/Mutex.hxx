//! \file Mutex.hxx
//! \brief Defines a mutex class that can optionally check for deadlick conditiona.
//! \details Compile with -DDEBUG to turn on deadlock checking
#ifndef MUTEX_HXX
#define MUTEX_HXX
#include <string>
#include <TMutex.h>
#include "Error.hxx"


namespace rb
{
  /// Simple derived class of TMutex that also allows users to check
  /// whether the current thred holds a lock on the mutex.
  class Mutex: public TMutex
  {
  public:
    static const Int_t kIsUnlocked = -1001;
  protected:
    //! Name associated with the mutex
    const std::string kName;
    //! Id of the thread holding the lock on this mutex
    Long_t fId;
  public:
    //! Sets name, recursive
    Mutex(const char* name = "", Bool_t recursive = kFALSE);
    //! Nothing to do explicitly
    virtual ~Mutex();
    //! \brief Locks fRootMutex
    virtual Int_t Lock();
    //! Mutex TryLock
    virtual Int_t TryLock();
    //! Unlock the mutex
    virtual Int_t UnLock();
    //! check if the mutex is locked by the current thread
    virtual Bool_t IsLocked();
    //! \returns fId
    Long_t GetId();
  private:
    //! Prevent copying
    Mutex(const Mutex& other) {}
    //! Prevent assignment
    Mutex& operator= (const Mutex& other) { return *this; }
  };

  /// Locks/unlocks the TThread global mutex [TThread::(Un)Lock]
  class TThreadMutex : public rb::Mutex
  {
  protected:
    //! Sets name
    TThreadMutex();
  public:
    //! Singleton instance function
    static TThreadMutex* Instance();
    //! Nothing to do
    virtual ~TThreadMutex();
    //! \brief TThread::Lock()
    virtual Int_t Lock();
    //! TThread::TryLock
    virtual Int_t TryLock();
    //! Unlock the mutex
    virtual Int_t UnLock();
  };   

  /// Class to lock a mutex upon construction and then unlock it upon destruction.
  /// \tparam M The type of mutex you want to use, must have a Lock() and UnLock()
  /// function otherwise you'll get a compile-time error.
  template <class M = rb::Mutex>
  class ScopedLock
  {
  private:
    //! Reference to the mutex you want to lock/unlock.
    M& fMutex;
  public:
    //! Initialize & lock fMutex
    ScopedLock(M& mutex);
    //! Initialize & lock fMutex, from pointer
    ScopedLock(M* mutex);
    //! Unlock fMutex
    ~ScopedLock();
  private:
    //! Prevent copying
    ScopedLock(const ScopedLock& other) {}
    //! Prevent assignment
    ScopedLock& operator= (const ScopedLock& other) { return *this; }
  };
}



// ======== Inline Function Implementations ========= //

// ======== Class rb::Mutex ========= //

inline rb::Mutex::Mutex(const char* name, Bool_t recursive):
  TMutex(recursive), kName(name), fId(kIsUnlocked) { }

inline rb::Mutex::~Mutex() { }

inline Int_t rb::Mutex::Lock() {
  Int_t ret = TMutex::Lock();
  fId = TThread::SelfId();
#ifdef RB_LOGGING
  RB_LOG << "  Locked:   " << kName << ", Thread ID: " << TThread::SelfId() << std::endl;
#endif
  return ret;
}

inline Int_t rb::Mutex::TryLock() {
  Int_t ret = TMutex::TryLock();
  if(ret == 0) {
    fId = TThread::SelfId();
#ifdef RB_LOGGING
  RB_LOG << "  Locked:   " << kName << ", Thread ID: " << TThread::SelfId() << std::endl;
#endif
  }
  return ret;
}

inline Int_t rb::Mutex::UnLock() {
  fId = kIsUnlocked;
#ifdef RB_LOGGING
    RB_LOG << "UnLocked: " << kName << ", Thread ID: " << TThread::SelfId() << std::endl;
#endif
  Int_t ret = TMutex::UnLock();
  return ret; 
}

inline Bool_t rb::Mutex::IsLocked() {
  return fId == TThread::SelfId();
}

inline Long_t rb::Mutex::GetId() {
  return fId;
}
  
// ======== Class rb::TThreadMutex ========= //

inline rb::TThreadMutex::TThreadMutex():
  rb::Mutex("TThreadGlobal", true) { }

inline rb::TThreadMutex::~TThreadMutex() {}

inline Int_t rb::TThreadMutex::Lock() {
  Int_t ret = TThread::Lock();
  fId = TThread::SelfId();
#ifdef RB_LOGGING
  RB_LOG << "  Locked:   " << kName << ", Thread ID: " << TThread::SelfId() << std::endl;
#endif
  return ret;
}

inline Int_t rb::TThreadMutex::TryLock() {
  Int_t ret = TThread::TryLock();
  if(ret == 0) {
    fId = TThread::SelfId();
#ifdef RB_LOGGING
    RB_LOG << "  Locked:   " << kName << ", Thread ID: " << TThread::SelfId() << std::endl;
#endif
  }
  return ret;
}

inline Int_t rb::TThreadMutex::UnLock() {
  fId = kIsUnlocked;
#ifdef RB_LOGGING
  RB_LOG << "UnLocked: " << kName << ", Thread ID: " << TThread::SelfId() << std::endl;
#endif
  Int_t ret = TThread::UnLock();
  return ret; 
}

inline rb::TThreadMutex* rb::TThreadMutex::Instance() {
  static rb::TThreadMutex* m = 0;
  if(!m) m = new rb::TThreadMutex();
  return m;
}

// ======= Class rb::ScopedLock ======= //

template <class M>
rb::ScopedLock<M>::ScopedLock(M& mutex): fMutex(mutex) {
  fMutex.Lock();
}

template <class M>
rb::ScopedLock<M>::ScopedLock(M* mutex): fMutex(*mutex) {
  fMutex.Lock();
}

template <class M>
rb::ScopedLock<M>::~ScopedLock() {
  fMutex.UnLock();
}



// ======= Globals ======= //

/// CINT Mutex
extern TVirtualMutex* gCINTMutex;

namespace rb {
  /// Data Mutex
  extern rb::Mutex gDataMutex;
}
namespace {
  /// TThread Mutex
  rb::Mutex* TTHREAD_GLOBAL_MUTEX = rb::TThreadMutex::Instance();
}

#define RB_LOCKGUARD(mutex)			\
  rb::ScopedLock<rb::Mutex> rb__lock(mutex)

#endif
