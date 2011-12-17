/// \file LockingPointer.hxx
/// \brief Implements a RAII-style locking pointer.
/// The basic idea behind these classes is nicely explained
/// <a href = "http://drdobbs.com/cpp/184403766">here</a>.
#ifndef __LOCKING_POINTER__
#define __LOCKING_POINTER__
#include <TMutex.h>


/// RAII mutex-locking pointer class.

//! This class allows access to objects shared between threads which have been
//! marked <tt>volatile</tt>, with pointer-like semantics.  When instantiated,
//! class automatically locks a mutex and automatically frees it when going out
//! of scope.  The basic idea behind how to use this class is something like this:
//! \code
//! volatile MyClass myclass; // Some object shared between threads that needs locking.
//! TMutex mtx; // The mutex we use to lock myclass
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
  /// Pointer to the critical object we want to access.
  T* fObject;

  /// Mutex used to lock the critical object.
  TMutex* fMutex;

public:
  /// Constructor (by reference).
  //! Set fObject and fMutex, lock fMutex.
  LockingPointer(volatile T& object, TMutex& mutex) :
    fObject(const_cast<T*>(&object)), fMutex(&mutex) {
    fMutex->Lock();
  }

  /// Constructor (by pointer).
  //! Set fObject and fMutex, lock fMutex. Included for convenience
  //! so we don't have to dereference things that are already pointers.
  LockingPointer(volatile T* object, TMutex& mutex) :
    fObject(const_cast<T*>(object)), fMutex(&mutex) {
    fMutex->Lock();
  }

  /// Destructor.
  //! Release the fMutex lock.
  ~LockingPointer() {
    fMutex->UnLock();
  }

  /// Return a pointer to the critical object.
  T* Get() {
    return fObject;
  }

  /// Return a pointer to the critical object.
  //! Same as Get(), but as an operator, allowing "real" pointer-like semantics.
  T* operator-> () {
    return fObject;
  }

  /// Return a reference to the critical object.
  T& operator* () {
    return *fObject;
  }

private:
  /// Copy constructor, does nothing.
  LockingPointer(const LockingPointer& other) { };

  /// Assignment operator, does nothing.
  LockingPointer& operator= (const LockingPointer& other) { };
};


/// Performs exactly the same functions as LockFreePointer but without any mutex locking.

//! Basically, this should only be used in cases where there is no need to lock the critical
//! object or when its mutex has already been locked. Example:
//! \code
//! volatile MyClass c1; // Some critical object.
//! volatile MyClass c2; // Some other critical object.
//! TMutex mtx; // Mutex protecting c1 and c2.
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
  /// Pointer to the critical object we want to access.
  T* fObject;

public:
  /// Constructor (by reference).
  //! Set fObject.
  LockFreePointer(volatile T& object) :
    fObject(const_cast<T*>(&object)) {  }

  /// Constructor (by pointer).
  //! Set fObject.
  LockFreePointer(volatile T* object) :
    fObject(const_cast<T*>(object)) {  }

  /// Descructor.
  //! Nothing to do.
  ~LockFreePointer() { }

  /// Return a pointer to the critical object.
  T* Get() {
    return fObject;
  }

  /// Return a pointer to the critical object.
  //! Same as Get(), but as an operator, allowing "real" pointer-like semantics.
  T* operator-> () {
    return fObject;
  }

  /// Return a reference to the critical object.
  T& operator* () {
    return *fObject;
  }

private:
  /// Copy constructor, does nothing.
  LockFreePointer(const LockFreePointer& other) { };

  /// Assignment operator, does nothing.
  LockFreePointer& operator= (const LockFreePointer& other) { };
};




#endif
