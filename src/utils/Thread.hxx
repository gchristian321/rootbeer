//! \file ThreadExecutor.hxx
//! \brief Defines an ABC for managing the running of threaded functions.
#ifndef THREAD_EXECUTOR_HXX
#define THREAD_EXECUTOR_HXX
#include <cassert>
#include <string>
#include <map>
#include <TThread.h>



//! \brief Manages the execution of threaded functions.
class ThreadExecutor
{
private:
  const char* fName;
  static std::map<std::string, ThreadExecutor*>& fgMap() {
    static std::map<std::string, ThreadExecutor*>* out =
      new std::map<std::string, ThreadExecutor*>();
    return *out;
  }
  TThread* fThread;

  //! Checks to make sure there's no duplicate thread names in the program.
  const char* NameCheck(const char* name) {
    bool ThreadNameIsUnique = TThread::GetThread(name) == 0;
    assert(ThreadNameIsUnique);
    return name;
  }

public:
  //! Initializes fThread
  ThreadExecutor(const char* name) : fName(name), fThread(0) {
    NameCheck(name);
  }

  //! The function that we want to run in the thread.
  virtual void DoInThread() = 0;

  //! Starts the thread
  inline Int_t Run() {
    fThread = new TThread(fName, ThreadExecutor::FRun);
    fgMap().insert(std::make_pair<std::string, ThreadExecutor*>(fName, this));
    return fThread->Run(reinterpret_cast<void*>(this));
  }

  //! Checks if a specific thread is running
  static Bool_t IsRunning(const char* name) {
    return fgMap().find(name) != fgMap().end();
  }

  //! Stops a specific thread from runing
  static void Stop(const char* name) {
    fgMap().erase(name);
  }

  //! Calls fThread->Join()
  virtual ~ThreadExecutor() {
    Stop(fName);
    if(fThread) {
      fThread->Join();
      delete fThread;
    }
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
