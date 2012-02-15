//! \file ThreadExecutor.hxx
//! \brief Defines an ABC for managing the running of threaded functions.
#ifndef THREAD_EXECUTOR_HXX
#define THREAD_EXECUTOR_HXX
#include <TThread.h>


//! \brief Manages the execution of threaded functions.
class ThreadExecutor
{
private:
  TThread fThread;

public:
  //! Initializes fThread
  ThreadExecutor(const char* name) :
    fThread(name, ThreadExecutor::FRun) {}

  //! Calls fThread->Join()
  virtual ~ThreadExecutor() {
    fThread.Join();
  }

  //! The function that we want to run in the thread.
  virtual void DoInThread() = 0;

  //! Starts the thread
  inline Int_t Run() {
    return fThread.Run(reinterpret_cast<void*>(this));
  }

private:
  //! \brief Function for passing to the TThread constructor.
  static void * FRun(void * args) {
    ThreadExecutor * this_ =  reinterpret_cast<ThreadExecutor*> (args);
    this_->DoInThread();
    delete this_;
  }

  //! Copy constructor (prevent copying)
  ThreadExecutor(const ThreadExecutor& other) { }

  //! Prevent equivalency.
  ThreadExecutor& operator= (const ThreadExecutor& other) { }
};

#endif
