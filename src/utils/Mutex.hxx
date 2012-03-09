//! \file Mutex.hxx
//! \brief Defines a mutex class that can optionally check for deadlick conditiona.
//! \details Compile with -DDEBUG to turn on deadlock checking
#ifndef MUTEX_HXX
#define MUTEX_HXX
#include <string>
#include <TMutex.h>
#include "Error.hxx"
#ifdef DEBUG
#include <cassert>
#include <cstdlib>
#include <list>
#include <utility>
#include <algorithm>
#include <TApplication.h>
#include <TError.h>
#endif

extern TVirtualMutex* gCINTMutex;

/*
namespace {
  inline Int_t gCINTMutex_Lock() {
    if(gCINTMutex) return gCINTMutex->Lock();
    return -1;
  }
  inline Int_t gCINTMutex_UnLock() {
    if(gCINTMutex) return gCINTMutex->UnLock();
  }
}
*/



namespace rb
{
  //! Class to lock a mutex upon construction and then unlock it upon destruction.
  //! \tparam M The type of mutex you want to use, must have a Lock() and UnLock()
  //! function otherwise you'll get a compile-time error.
  template <class M>
  class ScopedLock
  {
  private:
    //! Are we locking a local mutex or the global TThread one?
    const Bool_t kLocalMutex;
    //! Reference to the mutex you want to lock/unlock.
    M& fMutex;
  public:
    //! Initialize & lock fMutex
    ScopedLock(M& mutex): kLocalMutex(true), fMutex(mutex) {
      fMutex.Lock();
    }
    //! Initialize & lock fMutex, from pointer
    //! \note Passing NULL (or TTHREAD_GLOBAL) means to use the TThread global mutex
    ScopedLock(M* mutex): kLocalMutex(mutex != NULL), fMutex(*mutex) {
      if(kLocalMutex) fMutex.Lock();
      else {
	TThread::Lock();
	RB_LOG << "  TThread::Lock()" << std::endl;
      }
    }
    //! Unlock fMutex
    ~ScopedLock() {
      if(kLocalMutex) fMutex.UnLock();
      else {
	TThread::UnLock();
      	RB_LOG << "TThread::UnLock()" << std::endl;
      }
    }
  private:
    //! Prevent copying
    ScopedLock(const ScopedLock& other) {}
    //! Prevent assignment
    ScopedLock& operator= (const ScopedLock& other) {}
  };

  //! \brief Wrapper for TMutex that can optionally check for deadlock conditions.
  //! \details Compile with -DDEBUG to turn on deadlock checking. If deadlock is found,
  //! the applicion will terminate with an error.
  //! \note Use the LOCK_MUTEX macro for more useful debugging messages (including file
  //! and line from where the lock was attempted).
  class Mutex
  {
  private:
    const std::string kName;
    //! Internal ROOT mutex class.
    TMutex* fRootMutex;
#ifdef DEBUG
    Long_t fId;
#endif
  public:
    //! Allocates fRootMutex
    Mutex(const char* name = "", Bool_t recursive = kFALSE);
    //! Deallocates fRootMutex
    ~Mutex();
    //! \brief Locks fRootMutex
    //! \details If DEBUG is defined, checks first to see if the mutex
    //! is locked; adds <code>this</code> to fgLocked() when locking.
    Int_t Lock();
    //! Mutex TryLock
    Int_t TryLock();
    //! Unlock the mutex, (optionally) removes from fgLocked().
    Int_t UnLock();
#ifdef DEBUG
    //! Is the mutex recursive or not?
    const Bool_t kRecursive;
    //! Check if a particular mutex is locked.
    Bool_t IsLocked();
    //! \returns fId
    Long_t GetId();
#endif
  private:
    //! Prevent copying
    Mutex(const Mutex& other)
#ifdef DEBUG
      : kRecursive(0)
#endif
    {}
    //! Prevent assignment
    Mutex& operator= (const Mutex& other) { return *this; }
  };

#ifndef __MAKECINT__
  extern rb::Mutex gDataMutex;
#endif
}
namespace { rb::Mutex* const TTHREAD_GLOBAL_MUTEX (0); }

namespace
{
#ifdef DEBUG
  Long_t gLockedId = -1001;
  inline Bool_t IsLocked_TThread() {
    //    std::cout << gLockedId << " << gId : Self >> " << TThread::SelfId() << "\n";
    return gLockedId == TThread::SelfId();
  }
#endif

  inline Int_t Lock_TThread() {
#ifdef DEBUG
    assert(!rb::gDataMutex.IsLocked());
    assert(!IsLocked_TThread());
#endif
    Int_t ret = TThread::Lock();
#ifdef DEBUG
    gLockedId = TThread::SelfId();
#endif
    RB_LOG << std::endl;
    return ret;
  }

  inline Int_t UnLock_TThread() {
    Int_t ret = TThread::UnLock();
#ifdef DEBUG
    gLockedId = -1001;
#endif
    RB_LOG << std::endl;
    return ret;
  }
}


#ifndef __MAKECINT__ // Implemenation

inline rb::Mutex::Mutex(const char* name, Bool_t recursive) : kName(name)
#ifdef DEBUG
  , kRecursive(recursive), fId(-1001)
#endif
{
  RB_LOG << "Creating Mutex: name: " << kName << std::endl;
  fRootMutex = new TMutex(recursive);
}

inline rb::Mutex::~Mutex() {
  delete fRootMutex;
}

#ifndef DEBUG
inline Int_t rb::Mutex::Lock() {
  Int_t ret = fRootMutex->Lock();
  RB_LOG << "  Locked:   " << kName << ", Thread ID: " << TThread::SelfId() << std::endl;
  return ret;
}

inline Int_t rb::Mutex::TryLock() {
  return fRootMutex->TryLock();
}

inline Int_t rb::Mutex::UnLock() {
  Int_t ret = fRootMutex->UnLock();
  RB_LOG << "UnLocked: " << kName << ", Thread ID: " << TThread::SelfId() << std::endl;
  return ret; 
}

#else
inline Int_t rb::Mutex::Lock() {
  assert(!IsLocked());
  assert(!IsLocked_TThread());
  Int_t ret = fRootMutex->Lock();
  fId = TThread::SelfId();
  return ret;
}

inline Int_t rb::Mutex::TryLock() {
  return fRootMutex->TryLock();
}

inline Int_t rb::Mutex::UnLock() {
  fId = -1001;
  Int_t ret = fRootMutex->UnLock();
  return ret;
}

inline Bool_t rb::Mutex::IsLocked() {
  return fId == TThread::SelfId();
}

inline Long_t rb::Mutex::GetId() {
  return fId;
}
#endif // #ifdef DEBUG
  
#endif // #ifdef __MAKECINT__

#endif
