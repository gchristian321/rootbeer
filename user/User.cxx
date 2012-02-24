//! \file User.cxx
//! \brief For customizing rootbeer for a specific experiment.
//! \details Here is where users can tell rootbeer how to attach to data sources,
//! and how to read and unpack data buffers from those sources.  This is done by defining
//! a class that inherits from rb::BufferSource and implements all of its pure virtual methods.
//! For more information on those methods and what they should do, see the doxygen page on 
//! rb::BufferSource.  Additionally, users must implement the static rb::BufferSource::New()
//! method; this function shold return a \c new instance of the user's freshly created class
//! \code
//! rb::BufferSource* rb::BufferSource::New() {
//!    return new rb::MyBufferSourceDerivedClass();
//! }
//! \endcode
//! This is called in a few places (all in Buffer.cxx) where an instance of the <i>a priori</i>
//! unknown user-defined class is needed.
//!
//! Pre-defined classes are included with stock rootbeer for handling MIDAS (at TRIUMF) or
//! NSCL (spectrodaq) data. If this is what you're using for data acquisition, then all you should
//! need to do is to enable the appropriate branch of the code (by uncommenting the correct lines
//! in Makefile.user) and to implement the UnpackBuffer() member function in this file. Also, note
//! that in both the NSCL and MIDAS cases, the compiler checks if the extra software needed to connect
//! to an online data source is installed on your system. If it is, then the appropriate macros are 
//! #defined and the appropriate branches of the code are compiled. Otherwise, rootbeer will be
//! compiled with attaching to online data disabled.


// Standard includes, do not remove //
#include "Rootbeer.hxx"
#include "Buffer.hxx"
#include "Event.hxx"
#include "Data.hxx"
#include "Hist.hxx"


// Definition of a BufferSource class to handle MIDAS data (at TRIUMF). //
#ifdef MIDAS_BUFFERS
#include "midas/TMidasFile.h"
#ifdef MIDAS_ONLINE
#include "midas/TMidasOnline.h"
#endif
namespace rb
{
  class Midas : public rb::BufferSource
  {
  protected:
    Int_t fRequestId; //< Return code for online event requests.
    TMidasFile fFile; //< Offline MIDAS file.
    TMidasEvent fBuffer; //< Midas event buffer.
  public:
    Midas();
    virtual ~Midas();
    virtual Bool_t OpenFile(const char* file_name, char** other = 0, int nother = 0);
    virtual Bool_t ConnectOnline(const char* host, const char* other_arg = "", char** other_args = 0, int n_others = 0);
    virtual Bool_t ReadBufferOffline();
    virtual Bool_t ReadBufferOnline();
    virtual Bool_t UnpackBuffer();
    virtual void CloseFile();
    virtual void DisconnectOnline();
  protected:
    static void RunStop(int transition, int run_number, int trans_time);
    static void RunStart(int transition, int run_number, int trans_time);
    static void RunPause(int transition, int run_number, int trans_time);
    static void RunResume(int transition, int run_number, int trans_time);
  };
}

#include "vme/vme.hxx"
enum {
  DRAGON_EVENT = 1,
  DRAGON_SCALER = 2
};
class DragonEvent : public rb::Event
{
private:
  rb::data::Wrapper<Bgo> fBgo;
public:
  DragonEvent() : fBgo("bgo", this, true, "") {
  }
  ~DragonEvent() {}
private:
  TMidasEvent* Cast(void* addr) {return reinterpret_cast<TMidasEvent*>(addr);}
  Bool_t DoProcess(void* event_address, Int_t nchar);
  void HandleBadEvent() {Error("DragonEvent", "Something went wrong!!");}
};

rb::BufferSource* rb::BufferSource::New() {
  return new rb::Midas();
}

inline rb::Midas::Midas() : fRequestId(-1) {}

inline Bool_t rb::Midas::OpenFile(const char* file_name, char** other, int nother) {
  return fFile.Open(file_name);
}

inline Bool_t rb::Midas::ReadBufferOffline() {
  fBuffer.Clear();
  return fFile.Read(&fBuffer);
}

inline void rb::Midas::CloseFile() {
  fFile.Close();
}

inline void rb::Midas::DisconnectOnline() {
#ifdef MIDAS_ONLINE
  TMidasOnline::instance()->disconnect();
#endif
}  

inline rb::Midas::~Midas() {
  CloseFile();
  DisconnectOnline();
}

inline void rb::Midas::RunStart(int transition, int run_number, int trans_time) {
  Info("rb::Midas", "Starting run number %i.", run_number);
}

inline void rb::Midas::RunStop(int transition, int run_number, int trans_time) {
  Info("rb::Midas", "Stopping run number %i.", run_number);
}

inline void rb::Midas::RunPause(int transition, int run_number, int trans_time) {
  Info("rb::Midas", "Pausing run number %i.", run_number);
}

inline void rb::Midas::RunResume(int transition, int run_number, int trans_time) {
  Info("rb::Midas", "Resuming run number %i.", run_number);
}


Bool_t rb::Midas::ConnectOnline(const char* host, const char* experiment, char** unused, int unused2) {
#ifdef MIDAS_ONLINE
  TMidasOnline* onlineMidas = TMidasOnline::instance();
  Int_t err = onlineMidas->connect(host, experiment, "rootbeer");
  if (err) return kFALSE; // Message from TOnlineMidas::connect

  onlineMidas->setTransitionHandlers(RunStart, RunStop, RunPause, RunResume);
  onlineMidas->registerTransitions();
  fRequestId = onlineMidas->eventRequest("SYSTEM", -1, -1, (1<<1));
  return kTRUE;
#else
  return kFALSE;
#endif
}


Bool_t rb::Midas::ReadBufferOnline() {
#ifdef MIDAS_ONLINE
  Bool_t ret = kFALSE;
  TMidasOnline* onlineMidas = TMidasOnline::instance();
  char pEvent[100*1024];
  int size = 0;
  do { // loop until we get an error or event, or quit polling, or unattach
    size = onlineMidas->receiveEvent(fRequestId, pEvent, sizeof(pEvent), kTRUE);
  } while (size == 0 && rb::Thread::IsRunning(rb::attach::ONLINE_THREAD_NAME) && onlineMidas->poll(1000));

  if(size == 0) // Unattached or stopped polling
    ret = kFALSE;
  else if(size > 0) { // Got data, copy into midas event  
    //! \todo: byte ordering??
    memcpy(fBuffer.GetEventHeader(), pEvent, sizeof(EventHeader_t));
    fBuffer.SetData(size, pEvent+sizeof(EventHeader_t));
    ret = kTRUE;
  }    
  else { // Error reading event
    Error("ReadBufferOnline",  "onlineMidas->receiveEvent return val < 0.");
    ret = kFALSE;
  }
  return ret;
#else
  return kFALSE;
#endif
}

Bool_t rb::Midas::UnpackBuffer() {
#ifdef MIDAS_BUFFERS
  // (DRAGON test setup)
  Short_t eventId = fBuffer.GetEventId();
  //  fBgo->reset();

  //  CountedLockingPointer<Bgo> pBgo = fBgo.GetPointer();
  //  pBgo->test();

  switch(eventId) {
  case DRAGON_EVENT: // event
    {
      rb::Event* event = rb::Event::Instance<DragonEvent>();
      event->Process(&fBuffer, 0);
      //     fBgo->unpack(fBuffer);
      // fBgo->q[0] = 1000; ///fBgo2->q[0]; //1000;
	   //             printf("q[0]: %i\n", fBgo->q[0]);
    break;
    }
  case DRAGON_SCALER: // scaler
    break;
  default:
    //    Warning("UnpackBuffer", "Unrecognized Event Id: %d", eventId);
    break;
  }
  ////  rb::Hist::FillAll();
  return kTRUE;

#else
  return kFALSE;
#endif
}

Bool_t DragonEvent::DoProcess(void* addr, Int_t nchar) {
  TMidasEvent* fEvent = Cast(addr);
  if(fEvent) {
    fBgo->unpack(*fEvent);
    //    printf("fBgo->qraw[0]: %i\n", fBgo->qraw[0]);
    return true;
  }
  else return false;
}

void rb::Rint::RegisterEvents() {
  // Register events here //
  RegisterEvent<DragonEvent>(DRAGON_EVENT);
}

#else

// Throw a compile-time error.  The user should remove this one he/she has done what's required.
#error "You need to define a derived class of rb::BufferSource and implement rb::BufferSource::New()."

class /* ClassName */ : public rb::BufferSource()
{
  protected:
  // Data, etc.
  public:
  // Required functions //
    /*Name*/ (); // Constructor
    virtual ~/*Name*/ (); // Destructor
    virtual Bool_t OpenFile(const char* file_name, char** other = 0, int nother = 0);
    virtual Bool_t ConnectOnline(const char* host, const char* other_arg = "", char** other_args = 0, int n_others = 0);
    virtual Bool_t ReadBufferOffline();
    virtual Bool_t ReadBufferOnline();
    virtual Bool_t UnpackBuffer();
    virtual void CloseFile();
    virtual void DisconnectOnline();
  protected:
    // Internal functions, etc.
};

rb::BufferSource* rb::BufferSource::New() {
  // Needs to be implemented //

}



#endif 
