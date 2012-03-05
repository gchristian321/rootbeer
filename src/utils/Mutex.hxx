//! \file Mutex.hxx
//! \brief Defines a mutex class that can optionally check for deadlick conditiona.
//! \details Compile with -DDEBUG to turn on deadlock checking
#ifndef MUTEX_HXX
#define MUTEX_HXX
#include <TMutex.h>
#ifdef DEBUG
#include <cstdlib>
#include <list>
#include <utility>
#include <string>
#include <algorithm>
#include <TApplication.h>
#include <TError.h>
#endif

#define TTHREAD_GLOBAL_MUTEX 0
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
    ScopedLock(M* mutex): kLocalMutex(mutex != TTHREAD_GLOBAL_MUTEX), fMutex(*mutex) {
    if(kLocalMutex) fMutex.Lock();
    else TThread::Lock();
    }
    //! Unlock fMutex
    ~ScopedLock() {
    if(kLocalMutex) fMutex.UnLock();
    else TThread::UnLock();
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
    //! Internal ROOT mutex class.
    TMutex* fRootMutex;
#ifdef DEBUG
    typedef std::list<Long_t> List_t; 
    //! Map of all currently locked mutexes keying the thread id.
    List_t fLocked;
    // static List_t& fgLocked()  {
    //   static List_t* M = new List_t();
    //   return *M;
    // }
#endif
  public:
    //! Allocates fRootMutex
    Mutex(Bool_t recursive = kFALSE);
    //! Deallocates fRootMutex
    ~Mutex();
    //! \brief Locks fRootMutex
    //! \details If DEBUG is defined, checks first to see if the mutex
    //! is locked; adds <code>this</code> to fgLocked() when locking.
    Int_t Lock();
    //! Unlock the mutex, (optionally) removes from fgLocked().
    Int_t UnLock();
#ifdef DEBUG
    //! Is the mutex recursive or not?
    const Bool_t kRecursive;
    //! Check if a particular mutex is locked.
    Bool_t IsLocked();
#endif
  private:
    //! Prevent copying
    Mutex(const Mutex& other)
#ifdef DEBUG
      : kRecursive(0)
#endif
    {}
    //! Prevent assignment
    Mutex& operator= (const Mutex& other) {}
  };
  namespace { rb::Mutex gDataMutex; }
}

#ifndef __MAKECINT__ // Implemenation

inline rb::Mutex::Mutex(Bool_t recursive) 
#ifdef DEBUG
  : kRecursive(recursive)
#endif
{
  fRootMutex = new TMutex(recursive);
}

inline rb::Mutex::~Mutex() {
  delete fRootMutex;
}

#ifndef DEBUG
inline Int_t rb::Mutex::Lock() {
  return fRootMutex->Lock();
}

inline Int_t rb::Mutex::UnLock() {
  return fRootMutex->UnLock();
}

#else
namespace {
  TMutex gMutex__;
}
inline Int_t rb::Mutex::Lock() {
  gMutex__.Lock();
  if(!kRecursive && IsLocked()) {
    TThread * self = TThread::Self();
    std::string thName = self ? self->GetName() : "0";
    Error("rb::Mutex::Lock",
	  "Tried to lock a non-recursive mutex twice.\n"
	  "Thread Id: %ld, Name: %s", TThread::SelfId(), thName.c_str());
    gMutex__.UnLock();
    exit(1);
  }

  fLocked.push_back(TThread::SelfId());
  Int_t ret = fRootMutex->Lock();
  gMutex__.UnLock();
  return ret;
}

inline Int_t rb::Mutex::UnLock() {
  gMutex__.Lock();
  fLocked.remove(TThread::SelfId());
  Int_t ret = fRootMutex->UnLock();
  gMutex__.UnLock();
  return ret;
}

inline Bool_t rb::Mutex::IsLocked() {
  return std::find(fLocked.begin(), fLocked.end(), TThread::SelfId()) != fLocked.end();
}
#endif // #ifdef DEBUG
  
#endif // #ifdef __MAKECINT__






#if 0
class DebugMutex
{
private:
  //! Reference counter
  struct Counter {
    UInt_t fCount;
    Counter(UInt_t c = 1) :
      fCount(c) {}
  } * fCounter;
  TMutex* fMutex;

public:
  explicit DebugMutex() :
    fCounter(0) {
    fMutex = new TMutex();
  }
  //! Allocate a new counter
  Int_t Lock();
  //! Call Release()
  Int_t UnLock();

  ~DebugMutex() { Release(); }

private:
  //! Call Acquire
  DebugMutex(const DebugMutex& other) throw () {
    Acquire(other.fCounter);
  }
  //! Release this, acquire other.
  DebugMutex& operator= (const DebugMutex& other) {
    if (this != &other) {
      Release();
      Acquire(other.fCounter);
    }
    return *this;
  }

  //! Increment the count
  void Acquire(Counter* c) throw() {
    fCounter = c;
    if (c) ++c->fCount;
  }

  //! Decrement the count, delete if it is 0
  void Release() {
    if (fCounter) {
      if (--fCounter->fCount == 0) {
	//	delete fCounter->fPtr;
	fCounter->fMutex->UnLock();
	delete fCounter;
      }
      fCounter = 0;
    }
  }
};

#endif

#endif






