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
#include "User.hxx"
#include "utils/Error.hxx"


rb::BufferSource* rb::BufferSource::New() {
////////////////////////////////////////////////////////
// Need to fill in this function such that it returns //
// a new instance of your BufferSource class          //
////////////////////////////////////////////////////////

#ifdef MIDAS_BUFFERS	// Using rb::Midas as the BufferSource
	return new rb::Midas();
#else
#error "rb::BufferSource::New() needs to be implemented!"
#endif
}

#ifdef MIDAS_BUFFERS // Implementation of rb::Midas member functions

rb::Midas::Midas(): fRequestId(-1) { }

rb::Midas::~Midas() {
	CloseFile();
  DisconnectOnline();
}

Bool_t rb::Midas::OpenFile(const char*  file_name, char** other, int nother) {
  return fFile.Open(file_name);
}

void rb::Midas::CloseFile() {
  fFile.Close();
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

void rb::Midas::DisconnectOnline() {
#ifdef MIDAS_ONLINE
  TMidasOnline::instance()->disconnect();
#endif
}

Bool_t rb::Midas::ReadBufferOffline() {
  fBuffer.Clear();
  return fFile.Read(&fBuffer);
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
    err::Error("rb::Midas::ReadBufferOnline") << "onlineMidas->receiveEvent return val: " << size << ".";
    ret = kFALSE;
  }
  return ret;
#else
  return kFALSE;
#endif
}

Bool_t rb::Midas::UnpackBuffer() {
// This function depends on the user's experiment
#error "rb::Midas::UnpackBuffer() needs to be implemented!"
}

#endif // #ifdef MIDAS_BUFFERS

void rb::Rint::RegisterEvents() {
//////////////////////////////////////////////////////////////////
// Register your rb::Event derived classes here                 //
// Example:                                                     //
// RegisterEvent<EventClassName>(EVENT_CODE, "EventClassName"); //
//////////////////////////////////////////////////////////////////

#error "rb::Rint::RegisterEvents() needs to be implemented!"

}
