//! \file Skeleton.cxx
//! \brief This is where users plug their analysis codes into ROOTBEER.
#include "Skeleton.hxx"
#include "Data.hxx"
#include "Buffer.hxx"
#include "Rootbeer.hxx"
#ifdef MIDAS_BUFFERS
#include "midas/TMidasFile.h"
#ifdef MIDAS_ONLINE
#include "midas/TMidasOnline.h"
#endif
#endif
using namespace std;



//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
//                                   //
// ROOTBEER USER DATA CODE SKELETON  //
//                                   //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//


//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
//\\ Add the header includes for your classes here. //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

#include "ExampleData.hh"

#ifdef NSCL_BUFFERS
#include "mona.hh"
#include "unpacker.hh"
#include "mona_definitions.hh"
#endif

#ifdef MIDAS_BUFFERS
#include "vme/vme.hxx"
#endif



//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
//\\ Define instances of your class here    \\//
//\\ (using the ADD_CLASS_INSTANCE macros). \\//
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

// #if !defined NSCL_BUFFERS
// #if !defined MIDAS_BUFFERS
// Here we add an instance of ExampleData, called myData and do not allow viewing in CINT.
// ADD_CLASS_INSTANCE(myData, ExampleData, kFALSE)

// Here we add an instance of ExampleVariables, called myVars and do allow viewing in CINT.
// Also, we use a non-default constructor, with the argument 32.
// ADD_CLASS_INSTANCE_ARGS(myVars, ExampleVariables,  kTRUE, "32")
// ADD_CLASS_INSTANCE(myVars1, ExampleVariables,  kTRUE)
// #endif
// #endif

#ifdef NSCL_BUFFERS
ADD_CLASS_INSTANCE(mcal, cal::mona, kFALSE);
ADD_CLASS_INSTANCE(mraw, raw::mona, kFALSE);
ADD_CLASS_INSTANCE(mvar, var::mona, kTRUE);
#endif

#ifdef MIDAS_BUFFERS
//ADD_CLASS_INSTANCE(bgo, Bgo, kFALSE);
#endif



class Midas : public rb::BufferSource
{
protected:
  Int_t fRequestId; //< Return code for online event requests.
  TMidasFile fFile; //< Offline MIDAS file.
  TMidasEvent fBuffer; //< Midas event buffer.
  //  rb::TData<Bgo> fBgo;
public:
  Midas() : fRequestId(-1)/*, fBgo("bgo", false)*/ {}
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


Bool_t Midas::OpenFile(const char* file_name, char** other, int nother) {
  return fFile.Open(file_name);
}

Bool_t Midas::ReadBufferOffline() {
  fBuffer.Clear();
  return fFile.Read(&fBuffer);
}

Bool_t Midas::ConnectOnline(const char* host, const char* experiment, char** unused, int unused2) {
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

Bool_t Midas::ReadBufferOnline() {
#ifdef MIDAS_ONLINE
  Bool_t ret = kFALSE;
  TMidasOnline* onlineMidas = TMidasOnline::instance();
  char pEvent[100*1024];
  int size = 0;
  do { // loop until we get an error or event, or quit polling, or unattach
    size = onlineMidas->receiveEvent(fRequestId, pEvent, sizeof(pEvent), kTRUE);
  } while (size == 0 && IsConnected() && onlineMidas->poll(1000));

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

Midas::~Midas() {
}

void Midas::RunStart(int transition, int run_number, int trans_time) {
  cout << "Beginning MIDAS run number " << run_number << ".\n";
}

void Midas::RunStop(int transition, int run_number, int trans_time) {
  cout << "Ending MIDAS run number " << run_number << ".\n";
}

void Midas::RunPause(int transition, int run_number, int trans_time) {
  cout << "Pausing run number " << run_number << ".\n";
}

void Midas::RunResume(int transition, int run_number, int trans_time) {
  cout << "Resuming run number " << run_number << ".\n";
}
#include <TRandom3.h>
Bool_t Midas::UnpackBuffer() {
#ifdef MIDAS_BUFFERS
  // (DRAGON test setup)
  Short_t eventId = fBuffer.GetEventId();
  vme::Module::reset_all();

  AutoLockingPointer<Bgo> pBgo = fBgo->GetPointer();
  //  pBgo->test();

  switch(eventId) {
  case 1: // event
    //    vme::Module::unpack_all(fBuffer);
    pBgo->q[0] =  gRandom->Gaus(1000,50);
    break;
  case 2: // scaler
    break;
  default:
    //    Warning("UnpackBuffer", "Unrecognized Event Id: %d", eventId);
    break;
  }
  rb::Hist::FillAll();
  return kTRUE;

#else
  return kFALSE;
#endif
}


void Midas::CloseFile() {
  fFile.Close();
}

void Midas::DisconnectOnline() {
#ifdef MIDAS_ONLINE
  TMidasOnline::instance()->disconnect();
#endif
}  


rb::BufferSource* rb::BufferSource::GetFromUser() {
  return new Midas();
}
