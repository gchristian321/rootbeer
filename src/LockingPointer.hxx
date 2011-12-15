#ifndef __LOCKING_POINTER__
#define __LOCKING_POINTER__
#include <TThread.h>
#include <TMutex.h>
#include <assert.h>


static const Int_t kDeadlock(16);

template <typename T>
class LockingPointer
{
private:
  TMutex* fMutex;
  T* fObject;

public:
  LockingPointer(volatile T& object, TMutex& mutex) :
    fObject(const_cast<T*>(&object)), fMutex(&mutex) {
    TThread::Lock();
  }

  LockingPointer(volatile T* object, TMutex& mutex) :
    fObject(const_cast<T*>(object)), fMutex(&mutex) {
    TThread::Lock();
  }

  ~LockingPointer() {
    TThread::UnLock();
  }
  T* operator-> () {
    return fObject;
  }
  T& operator* () {
    return *fObject;
  }

private:
  LockingPointer(const LockingPointer& other) { };
  LockingPointer& operator= (const LockingPointer& other) { };
};


template <typename T>
class LockFreePointer
{
private:
  T* fObject;

public:
  LockFreePointer(volatile T& object) :
    fObject(const_cast<T*>(&object)) {  }

  LockFreePointer(volatile T* object) :
    fObject(const_cast<T*>(object)) {  }

  ~LockFreePointer() { }

  T* operator-> () {
    return fObject;
  }
  T& operator* () {
    return *fObject;
  }

private:
  LockFreePointer(const LockFreePointer& other) { };
  LockFreePointer& operator= (const LockFreePointer& other) { };
};




#endif
