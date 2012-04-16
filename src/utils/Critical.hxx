#ifndef CRITICAL_HXX
#define CRITICAL_HXX
//! \file Critical.hxx
//! \brief Defines a class for ensuring that critical elements are
//! locked when accesed.
#include <cassert>
#include "Mutex.hxx"
#include "nocopy.h"

namespace rb
{
template <class T>
class Critical
{
	 RB_NOCOPY(Critical);
private:
	 T* fCritical;
	 rb::Mutex& fMutex;
public:
	 Critical (T* critical, rb::Mutex& mutex);
	 Critical (T* critical, rb::Mutex* mutex);
	 ~Critical();
	 T* Get();
	 T* get();
	 T* operator-> ();
	 T& operator* ();
	 void Reset(T*);
};
}

template <class T>
rb::Critical<T>::Critical(T* critical, rb::Mutex& mutex):
  fCritical(critical), fMutex(mutex) { }

template <class T>
rb::Critical<T>::Critical(T* critical, rb::Mutex* mutex):
  fCritical(critical), fMutex(*mutex) { }

template <class T>
rb::Critical<T>::~Critical() {
  if(fCritical) delete fCritical;
}

template <class T>
T* rb::Critical<T>::Get() {
#ifdef DEBUG
  assert(fMutex.IsLocked());
#endif
  return fCritical;
}

template <class T>
T* rb::Critical<T>::get() {
  return Get();
}

template <class T>
T* rb::Critical<T>::operator->() {
  return Get();
}

template <class T>
T& rb::Critical<T>::operator*() {
#ifdef DEBUG
  assert(fCritical != NULL);
#endif
  return *Get();
}

template <class T>
void rb::Critical<T>::Reset(T* new_) {
	if(fCritical) delete fCritical;
	fCritical = new_;
}


#endif
