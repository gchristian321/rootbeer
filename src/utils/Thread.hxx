//! \file Thread.hxx
//! \brief Defines an ABC for managing the running of threaded processes.
#ifndef THREAD_HXX
#define THREAD_HXX
#include <cassert>
#include <string>
#include <set>
#include <map>
#include <TCint.h>
#include <TThread.h>
#include <TCondition.h>
#include "Mutex.hxx"
#include "nocopy.h"

namespace rb
{
//! \brief Manages the execution of threaded functions.
//! \details This class allows for threads to be run under the RAII principle, i.e.
//! via a class that automatically handles it's own initialization and destruction
//! of running in the threaded environment. This is an abstract base class, with th
//! pure virtual function DoInThread() that must be implemented in derived classes.
//! To use, simply create a new instance of a derived class and then call Run().
//! \code
//! rb::Thread* thread = new MyThreadedClass();
//! thread->Run();
//! \endcode
//! \warning For proper use, you must allocate onto the heap using <tt>new</tt>. Otherwise,
//! if the rb::Thread instance goes out of scope wile the threaded function is still
//! running, there will be major problems since the destructor is called upon returning
//! from DoInThread().  To avoid accidental stack allocation, you can make the constructor
//! of your derived classes private or protected and then add a static New() or Create() function
//! that returns a pointer to heap-allocated instance (or a CreateAndRun() function that crates and
//! instance on the heap and also calls Run()).
class Thread
{
	 RB_NOCOPY(Thread);
public:
	 typedef std::map<std::string, rb::Thread*> Set_t;

protected:
	 //! Name of the thread
	 //! \note Must be unique; duplicate names result in an assert.
	 const char* fName;

private:
	 //! Keeps track of all presently <it>running</it> threads.
	 static Thread::Set_t& fgSet() {
		 static Thread::Set_t* out = new Thread::Set_t();
		 return *out;
	 }

	 //! ROOT TThread, handles the running of the threaded function.
	 TThread* fThread;

	 //! Checks to make sure there's no duplicate thread names in the program,
	 //! throws an assert if there is.
	 //! \todo Maybe it would be better to use an exception?
	 const char* NameCheck(const char* name) {
		 bool ThreadNameIsUnique = TThread::GetThread(name) == 0;
		 assert(ThreadNameIsUnique);
		 return name;
	 }

	 Bool_t kInLoop;
	 TCondition fInLoop;

public:
	 //! \details Sets fName, initializes fThread to 0, checks for duplicate names.
	 Thread(const char* name) : fName(name), fThread(0), kInLoop(false) {
		 NameCheck(name);
	 }

	 //! \details Calls Stop(), TThread::Join() on fThread, deallocated fThread.
	 virtual ~Thread() {
		 Stop(fName);
		 if(fThread) {
			 //	fThread->Join();
			 delete fThread;
		 }
	 }

	 //! Returns the thread name
	 std::string GetName() { return std::string(fName); }

	 //! \brief Function that we want to run in a threaded environment.
	 //! \details As this is pure virtual, it must be implemented in derived
	 //! classes. This is where we tell what we want to actually happen in the
	 //! threaded environment.
	 virtual void DoInThread() = 0;

	 //! \brief Start running the thread.
	 Int_t Run() {
		 fThread = new TThread(fName, Thread::FRun);
		 fgSet().insert(std::make_pair<std::string, rb::Thread*> (fName, this));
		 return fThread->Run(reinterpret_cast<void*>(this));
	 }

	 //! \brief Checks if a specific thread is running (see Stop() for more details).
	 static Bool_t IsRunning(const char* name) {
		 return fgSet().count(name);
	 }

	 static rb::Thread* GetThread(const char* name) {
		 return IsRunning(name) ? fgSet().find(name)->second : 0;
	 }

	 //! Stop running a specific thread.
	 //! \details This function, along with IsRunning() are meant to allow
	 //! threads to be terminated by an external signal.  To do this, place
	 //! a condition on IsRunning within the implementation of DoInThread(),
	 //! then an external call to TThread::Stop() can stop the thread execution.
	 //! For example:
	 //! \code
	 //! // In MyThread::DoInThread()
	 //! while TThread::IsRuning("MyThreadName") // do something
	 //! // .... //
	 //! // Now in some external function
	 //! if (whatever) TThread::Stop("MyThreadName") // breaks out of the DoInThread() loop.
	 //! \endcode
	 static void Stop(const char* name) {
		 if(IsRunning(name)) {
			 rb::Thread* this_ = rb::Thread::GetThread(name);
			 fgSet().erase(name);
			 TThread* th = TThread::GetThread(name);
			 if(th) {
				 Int_t unlock = gCINTMutex ? gCINTMutex->UnLock() : 1;
				 th->Join();
				 if(!unlock) gCINTMutex->Lock();
				 delete this_;
			 }
		 }
	 }

private:
	 //! \brief Function for passing to the TThread constructor.
	 //! \details TThread requires a function <tt>void * func(void *)</tt>
	 //! Here we define one that takes (void*)this as the argument, allowing
	 //! us to run a normal member function [DoInThread()] in the threaded
	 //! environment.  DoInThread() returns, call the destructor on this,
	 //! allowing for self-management of allocated memory.
	 static void * FRun(void * args) {
		 Thread * this_ =  reinterpret_cast<Thread*> (args);
		 this_->DoInThread();
		 Stop(this_->fName);
		 return 0;
	 }
};
}

#endif
