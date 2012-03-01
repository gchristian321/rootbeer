//! \file LockingPointer.hxx
//! \brief defines a RAII-style locking pointer.
//! \details The basic idea behind these classes is nicely explained
//! <a href = "http://drdobbs.com/cpp/184403766">here</a>.
#ifndef LOCKING_POINTER_HXX
#define LOCKING_POINTER_HXX
#include "utils/Mutex.hxx"
// #define LOCKING_POINTER_VERBOSE

//! RAII mutex-locking pointer class.

//! This class allows access to objects shared between threads which have been
//! marked <tt>volatile</tt>, with pointer-like semantics.  When instantiated,
//! class automatically locks a mutex and automatically frees it when going out
//! of scope.  The basic idea behind how to use this class is something like this:
//! \code
//! volatile MyClass myclass; // Some object shared between threads that needs locking.
//! rb::Mutex mtx; // The mutex we use to lock myclass
//! 
//! void someThreadedFunction() {
//!    LockingPointer<MyClass> p (myclass, mtx); // Get access to 'myclass', locked by 'mtx'.
//!    p->SomeMemberFunctionOfMyClass();
//!    ...
//!    } // 'p' goes out of scope, releases the lock.
//! \endcode
//! Note that if we had tried to get access to \c myclass directly,
//! \code
//!    myclass.SomeMemberFunctionOfMyClass();
//! \endcode
//! the compiler wouldn't have allowed it since myclass was declared <tt>volatile</tt>.
//! This is a good thing sice direct access to \c myclass is not thread safe. So by declaring
//! critical objects \c volatile and accessing via LockingPointer, we let the compiler prevent
//! us from screwing up. For more information: http://drdobbs.com/cpp/184403766
//!
//! Similar functionality (casting away volatile) but without the mutex locking can be had using 
//! LockFreePointer (but use with care...). 
template <typename T>
class LockingPointer
{
private:
  //! Pointer to the critical object we want to access.
  T* fObject;

  //! Mutex used to lock the critical object.
  rb::Mutex* fMutex;

public:
  //! Constructor (by reference).
  //! Set fObject and fMutex, lock fMutex.
  LockingPointer(volatile T& object, rb::Mutex& mutex) :
    fObject(const_cast<T*>(&object)), fMutex(&mutex) {
    fMutex->Lock();
#ifdef LOCKING_POINTER_VERBOSE
    Info("LockingPointer", "Locking");
#endif
  }

  //! Constructor (by pointer).
  //! Set fObject and fMutex, lock fMutex. Included for convenience
  //! so we don't have to dereference things that are already pointers.
  LockingPointer(volatile T* object, rb::Mutex& mutex) :
    fObject(const_cast<T*>(object)), fMutex(&mutex) {
    fMutex->Lock();
#ifdef LOCKING_POINTER_VERBOSE
    Info("LockingPointer", "Locking");
#endif
  }

  //! Constructor (by reference and mutex pointer).
  //! Set fObject and fMutex, lock fMutex. If the mutex argument is TTHREAD_GLOBAL_MUTEX,
  //! Lock the TThread global mutex
  LockingPointer(volatile T& object, rb::Mutex* mutex) :
    fObject(const_cast<T*>(&object)), fMutex(mutex) {
    if(fMutex) fMutex->Lock();
    else TThread::Lock();
#ifdef LOCKING_POINTER_VERBOSE
    Info("LockingPointer", "Locking");
#endif
  }

  //! Constructor (by pointer and mutex pointer).
  //! Set fObject and fMutex, lock fMutex. If the mutex argument is TTHREAD_GLOBAL_MUTEX,
  //! Lock the TThread global mutex
  LockingPointer(volatile T* object, rb::Mutex* mutex) :
    fObject(const_cast<T*>(object)), fMutex(mutex) {
    if(fMutex) fMutex->Lock();
    else TThread::Lock();
#ifdef LOCKING_POINTER_VERBOSE
    Info("LockingPointer", "Locking");
#endif
  }

  //! Destructor.
  //! Release the fMutex lock.
  ~LockingPointer() {
    if(fMutex) fMutex->UnLock();
    else TThread::UnLock();
#ifdef LOCKING_POINTER_VERBOSE
    Info("~LockingPointer", "UnLocking");
#endif
  }

  //! Return a pointer to the critical object.
  T* Get() {
    return fObject;
  }

  //! Return a pointer to the critical object.
  //! Same as Get(), but as an operator, allowing "real" pointer-like semantics.
  T* operator-> () {
    return fObject;
  }

  //! Return a reference to the critical object.
  T& operator* () {
    return *fObject;
  }

private:
  //! Copy constructor, does nothing.
  LockingPointer(const LockingPointer& other) { };

  //! Assignment operator, does nothing.
  LockingPointer& operator= (const LockingPointer& other) { };
};


//! Performs exactly the same functions as LockFreePointer but without any mutex locking.

//! Basically, this should only be used in cases where there is no need to lock the critical
//! object or when its mutex has already been locked. Example:
//! \code
//! volatile MyClass c1; // Some critical object.
//! volatile MyClass c2; // Some other critical object.
//! rb::Mutex mtx; // Mutex protecting c1 and c2.
//! void function() {
//!    LockingPointer<MyClass> p1(c1, mtx); // locks mtx.
//!    LockFreePointer<MyClass> p2(c2, mtx); // gives access to c2, no need to lock
//!                                          // because p1 did already.
//!    p1->DoSomething();
//!    p2->DoSomethingElse();
//! }
//! \endcode
//! \warning Use with care....

template <typename T>
class LockFreePointer
{
private:
  //! Pointer to the critical object we want to access.
  T* fObject;

public:
  //! Constructor (by reference).
  //! Set fObject.
  LockFreePointer(volatile T& object) :
    fObject(const_cast<T*>(&object)) {  }

  //! Constructor (by pointer).
  //! Set fObject.
  LockFreePointer(volatile T* object) :
    fObject(const_cast<T*>(object)) {  }

  //! Descructor.
  //! Nothing to do.
  ~LockFreePointer() { }

  //! Return a pointer to the critical object.
  T* Get() {
    return fObject;
  }

  //! Return a pointer to the critical object.
  //! Same as Get(), but as an operator, allowing "real" pointer-like semantics.
  T* operator-> () {
    return fObject;
  }

  //! Return a reference to the critical object.
  T& operator* () {
    return *fObject;
  }

private:
  //! Copy constructor, does nothing.
  LockFreePointer(const LockFreePointer& other) { };

  //! Assignment operator, does nothing.
  LockFreePointer& operator= (const LockFreePointer& other) { };
};


//! \brief Reference counting LockingPointer
//! \details Like a LockingPointer, except it releases the mutex when going
//! out of scope and can be coiped.
template <class T>
class CountedLockingPointer
{
private:
  //! Reference counter
  struct Counter {
    T* fPtr;
    rb::Mutex* fMutex;
    UInt_t fCount;
    Counter(volatile T* p = 0, rb::Mutex* m = 0, UInt_t c = 1) :
      fPtr(const_cast<T*>(p)), fCount(c) {
      fMutex = m;
      if(fMutex) fMutex->Lock();
#ifdef LOCKING_POINTER_VERBOSE
	Info("CountedLockingPointer", "Locking");
#endif
    }
  } * fCounter;

public:
  //! Allocate a new counter
  explicit CountedLockingPointer(volatile T* object = 0, rb::Mutex* mutex = 0) :
    fCounter(0) {
    if (object) fCounter = new Counter(object, mutex);
  }
  //! Allocate a new counter
  explicit CountedLockingPointer(volatile T& object, rb::Mutex* mutex) :
    fCounter(0) {
    fCounter = new Counter(&object, mutex);
  }
  //! Call Release()
  ~CountedLockingPointer() { Release(); }
  //! Call Acquire
  CountedLockingPointer(const CountedLockingPointer& other) throw () {
    Acquire(other.fCounter);
  }
  //! Release this, acquire other.
  CountedLockingPointer& operator= (const CountedLockingPointer& other) {
    if (this != &other) {
      Release();
      Acquire(other.fCounter);
    }
    return *this;
  }

  //! Return a pointer to the critical object.
  T* Get() const throw() {
    return  fCounter ? fCounter->fPtr : 0;
  }

  //! Return a pointer to the critical object.
  //! Same as Get(), but as an operator, allowing "real" pointer-like semantics.
  T* operator-> () const throw() {
    return  fCounter->fPtr;
  }

  //! Return a reference to the critical object.
  T& operator* () const throw() {
    return *fCounter->fPtr;
  }

  //! Check if no copies are made.
  Bool_t IsUnique() const throw() {
    return (fCounter ? fCounter->fCount == 1 : kTRUE);
  }

private:
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
#ifdef LOCKING_POINTER_VERBOSE
	Info("CountedLockingPointer", "UnLocking");
#endif
	delete fCounter;
      }
      fCounter = 0;
    }
  }
};

#endif
