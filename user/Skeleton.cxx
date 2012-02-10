//! \file Skeleton.cxx
//! \brief This is where users plug their analysis codes into ROOTBEER.
#include "Skeleton.hxx"
#include "Data.hxx"
#include "Rootbeer.hxx"
#include "midas/TMidasFile.h"
#ifdef MIDAS_ONLINE
#include "midas/TMidasOnline.h"
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

#ifdef _NSCL_
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

// #if !defined _NSCL_
// #if !defined MIDAS_BUFFERS
// Here we add an instance of ExampleData, called myData and do not allow viewing in CINT.
// ADD_CLASS_INSTANCE(myData, ExampleData, kFALSE)

// Here we add an instance of ExampleVariables, called myVars and do allow viewing in CINT.
// Also, we use a non-default constructor, with the argument 32.
// ADD_CLASS_INSTANCE_ARGS(myVars, ExampleVariables,  kTRUE, "32")
// ADD_CLASS_INSTANCE(myVars1, ExampleVariables,  kTRUE)
// #endif
// #endif

#ifdef _NSCL_
ADD_CLASS_INSTANCE(mcal, cal::mona, kFALSE);
ADD_CLASS_INSTANCE(mraw, raw::mona, kFALSE);
ADD_CLASS_INSTANCE(mvar, var::mona, kTRUE);
#endif

#ifdef MIDAS_BUFFERS
//ADD_CLASS_INSTANCE(bgo, Bgo, kFALSE);
#endif


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
  } while (size == 0 && IsAttached(ONLINE_) && onlineMidas->poll(1000));

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
#ifdef MIDAS_ONLINE
  TMidasOnline::instance()->disconnect();
#endif
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

Bool_t Midas::UnpackBuffer() {
#ifdef MIDAS_BUFFERS
  // (DRAGON test setup)
  //  GET_LOCKING_POINTER(pBgo, bgo, Bgo);

  //  auto_ptr<LockingPointer<Bgo> > p = ((rb::TData<Bgo>*)bgo_Data)->GetLockedData();
  //  LockingPointer<CLASS>& SYMBOL = *temp_12312312312;

  Short_t eventId = fBuffer.GetEventId();
  vme::Module::reset_all();
  switch(eventId) {
  case 1: // event
    vme::Module::unpack_all(fBuffer);
    break;
  case 2: // scaler
    break;
  default:
    Warning("UnpackBuffer", "Unrecognized Event Id: %d", eventId);
    break;
  }
  rb::Hist::FillAll();
  return kTRUE;
#else
  return kFALSE;
#endif
}


void Midas::AddData() {
  ADD_CLASS_INSTANCE_ARGS(myVars, ExampleVariables, kTRUE, "32");
  ADD_CLASS_INSTANCE(myData, ExampleData, kTRUE);
  fUserData.push_back(myVars_Data);
  fUserData.push_back(myData_Data);
}








/*
#if 0
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
//\\ Here you should define how to process your data buffers        \\//
//\\ by implementing the ReadBuffer() and UnpackBuffer() functions. \\//
//\\ For more information, see the doxygen page on rb::unpack       \\//
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
bool rb::unpack::ReadBuffer(int size, void* from, BUFFER_TYPE& buffer) {

#ifdef MIDAS_BUFFERS
  //! \todo: Memcpy OK? Byte swapping?
  char* pdata = (char*)from;
  UShort_t headerSize = sizeof(EventHeader_t);
  if(size < headerSize) {
    Warning("ReadBuffer", "buffer size (%d) < event header size (%d), skipping",
	    size, headerSize);
    return false;
  }

  memcpy(buffer.GetEventHeader(), pdata, headerSize);
  buffer.SetData(size-headerSize, pdata+headerSize);

  //  buffer.SetBankList();
  //  bool result = buffer.ReadEvent(&ifs);
  return true; //result;
#elif defined _NSCL_
  const Int_t bufferSize = 4096;
  if(buffer.size() != bufferSize) buffer.resize(bufferSize);
  ifs.read(reinterpret_cast<Char_t*>(&buffer[0]), bufferSize * sizeof(DATA_TYPE));
  return ifs.good();
#else
  // Define your own buffer extraction method
  // Be sure to return a bool noting whether the extraction was
  // successful or not.

#endif
}
#endif

void rb::unpack::UnpackBuffer(BUFFER_TYPE& buffer) {

#ifdef MIDAS_BUFFERS

  //  GET_LOCKING_POINTER(pBgo, bgo, Bgo);
  int16_t eventId = buffer.GetEventId();
  bool unpackResult;
  vme::Module::reset_all();
  switch(eventId) {
  case 1:
    vme::Module::unpack_all(buffer);
    break;
  default:
    //    Info("UnpackBuffer", "Event Id: %d", eventId);
    break;
  }
  rb::Hist::FillAll();

#elif defined _NSCL_

  static const UInt_t EVENT_BUFFER = 1;
  static const UInt_t BUF_HEADER_LENGTH = 16;
  static unsigned short* pEvt, nEvts, evtLength;
  static int nBuffers = 0;
  static raw::unpacker unpack;

  unpack.read_online_buffer((UShort_t*)&buffer[0]);
  if(unpack.get_buffer_type() == EVENT_BUFFER)
    {
      ++nBuffers;

      int runnum = unpack.get_run_number();
      nEvts = unpack.get_n_evts();
      pEvt  = unpack.get_buffer();
      pEvt += BUF_HEADER_LENGTH;

      std::vector<unsigned short> monalisa_packets;
      monalisa_packets.push_back(MONA_PACKET);
      monalisa_packets.push_back(LISA_PACKET);

      GET_LOCKING_POINTER(p_mraw, mraw, raw::mona);
      GET_LOCKING_POINTER(p_mcal, mcal, cal::mona);
      GET_LOCKING_POINTER(p_mvar, mvar, var::mona);

      for(unsigned short i=0; i< nEvts; ++i) //\loop events
  	{
  	  evtLength = *pEvt;
  	  unpack.unpack_detector(p_mraw.Get(), monalisa_packets, pEvt);
  	  p_mcal->calibrate(p_mraw.Get(), *p_mvar);

  	  rb::Hist::FillAll();//
  	  // Move on to the next event
  	  pEvt += evtLength;

  	}
    }

#else

  UShort_t* p = &buffer[0]; // Point to the beginning of the buffer
  Int_t nEvts = *p++;   // Figure out the number of events
  GET_LOCKING_POINTER(pData, myData, ExampleData);
  GET_LOCKING_POINTER(pVars, myVars, ExampleVariables);


  for (Int_t event = 0; event < nEvts; ++event) { // Loop over all events


    p += pData->process_event(p);   // Process each event using external user code
    pData->calibrate(*pVars);   // Calibrate everything for this event





    //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
    // NOTE: You need to include this call to rb::Hist::FillAll() at the end  //
    // of your event loop if you want to be able to see histogrammed data.    //
    //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
    //\\\\\\\\\\\\\\\\\\\\\//
    rb::Hist::FillAll();//
    //\\\\\\\\\\\\\\\\\\\\\//

  } // End event loop

#endif

} // End function
*/
