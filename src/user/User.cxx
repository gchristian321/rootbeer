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

// #include <midas.h>
// #include "utils/Error.hxx"
// #include "midas/Database.hxx"

// void dbtest()
// {

// 	midas::Database db("online");
// 	double slope = -1;
// 	db.ReadValue("/dragon/head/variables/xtdc/slope", slope);
// 	printf("slope: %f\n", slope);


// 	// int hndle;
	
// 	// printf("Exists? %i [ CM_SUCCESS = %i, CM_NO_CLIENT = %i ]\n", cm_exist("rootbeer", TRUE), CM_SUCCESS, CM_NO_CLIENT);

// 	// cm_get_experiment_database(&hndle, 0);
// 	// if (hndle == 0)
// 	// 	err::Error("midas::Odb") << "Not connected to an experiment\n";

// }


#if 0


#include "User.hxx"
#include "utils/Error.hxx"
#include "dragon/definitions.h"

rb::BufferSource* rb::BufferSource::New() {
  return new rb::Midas();
}

Bool_t rb::Midas::ReadBufferOffline() {
	TMidasEvent temp;
	bool ret = fFile.Read(&temp);
	if (ret) {
		memcpy (fBuffer, temp.GetEventHeader(), sizeof(EventHeader_t));
		memcpy (fBuffer + sizeof(EventHeader_t), temp.GetData(), temp.GetDataSize());
	}
	return ret;
}

Bool_t rb::Midas::ConnectOnline(const char* host, const char* experiment, char** unused, int unused2) {
#ifdef MIDAS_ONLINE
  TMidasOnline* onlineMidas = TMidasOnline::instance();
  int err = onlineMidas->connect(host, experiment, "rootbeer");
  if (err) return kFALSE; // Message from TOnlineMidas::connect

  onlineMidas->setTransitionHandlers(RunStart, RunStop, RunPause, RunResume);
  onlineMidas->registerTransitions();
  fRequestId = onlineMidas->eventRequest("SYNC", -1, -1, (1<<1));
	return kTRUE;
#else
  return kFALSE;
#endif
}


namespace { inline bool isOnlineAttached() { return rb::Thread::IsRunning(rb::attach::ONLINE_THREAD_NAME); } }

Bool_t rb::Midas::ReadBufferOnline() { /// \todo figure out poll length
#ifdef MIDAS_ONLINE
  Bool_t ret = kFALSE;
  TMidasOnline* onlineMidas = TMidasOnline::instance();
//  char pEvent[100*1024];

	const int poll_length = 0;

  int size = 0;
  do { // loop until we get an error or event, or quit polling, or unattach
    size = onlineMidas->receiveEvent(fRequestId, fBuffer, sizeof(fBuffer), kTRUE);
  } while (size == 0 && rb::Thread::IsRunning(rb::attach::ONLINE_THREAD_NAME) && onlineMidas->poll(poll_length));

  if(size == 0) // Unattached or stopped polling
		 ret = kFALSE;
  else if(size > 0) { // Got data, copy into midas event  
    //! \todo: byte ordering??
    // memcpy(fBuffer.GetEventHeader(), pEvent, sizeof(EventHeader_t));
    // fBuffer.SetData(size, pEvent+sizeof(EventHeader_t));
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
namespace { 	DragonQueue gQueue(10e6);  }

void rb::Midas::FlushQueue() {
	gQueue.Flush();
}

void rb::Midas::RunStop(int transition, int run_number, int trans_time) {
	gQueue.Flush();
  Info("rb::Midas", "Stopping run number %i.", run_number);
}

Bool_t rb::Midas::UnpackBuffer() {

#ifdef MIDAS_BUFFERS
  // (DRAGON test setup)
  Short_t eventId = ((EventHeader_t*)fBuffer)->fEventId;
  switch(eventId) {
  case DRAGON_HEAD_EVENT:
		gQueue.Push(dragon::MidasEvent("TSCH", fBuffer, ((EventHeader_t*)fBuffer)->fDataSize));
		break;
	case DRAGON_TAIL_EVENT:
		gQueue.Push(dragon::MidasEvent("TSCT", fBuffer, ((EventHeader_t*)fBuffer)->fDataSize));
		break;
  case DRAGON_HEAD_SCALER: // scaler
		///\ todo Implement chris's stuff
    break;
  case DRAGON_TAIL_SCALER: // scaler
		///\ todo Implement chris's stuff
    break;
  default:
    break;
  }
  return kTRUE;
#else
  return kFALSE;
#endif
}

CoincidenceEvent::CoincidenceEvent(): fDragon("coinc", this, false, "") { }

Bool_t CoincidenceEvent::DoProcess(const void* addr, Int_t nchar) {
	const dragon::CoincMidasEvent* coinc = Cast(addr);

	dragon::gamma::Gamma gamma;
	gamma.unpack(*(coinc->fGamma));
	gamma.read_data();
	gamma.calculate();

	dragon::hion::HeavyIon heavyIon;
	heavyIon.unpack(*(coinc->fHeavyIon));
	heavyIon.read_data();
	heavyIon.calculate();

	fDragon->read_event(gamma, heavyIon);

	return true;
}

GammaEvent::GammaEvent(): fGamma("gamma", this, true, "") { }

Bool_t GammaEvent::DoProcess(const void* addr, Int_t nchar) {
  const dragon::MidasEvent* fEvent = Cast(addr);
  if(fEvent) {
		fGamma->unpack(*fEvent);
		fGamma->read_data();
		fGamma->calculate();
    return true;
  }
  else return false;
}

HeavyIonEvent::HeavyIonEvent(): fHeavyIon("hi", this, true, "") { }

Bool_t HeavyIonEvent::DoProcess(const void* addr, Int_t nchar) {
  const dragon::MidasEvent* fEvent = Cast(addr);
  if(fEvent) {
		fHeavyIon->unpack(*fEvent);
		fHeavyIon->read_data();
		fHeavyIon->calculate();
    return true;
  }
  else return false;
}

void rb::Rint::RegisterEvents() {
  // Register events here //
  RegisterEvent<CoincidenceEvent>(COINCIDENCE_EVENT, "CoincidenceEvent");
	RegisterEvent<GammaEvent>(GAMMA_EVENT, "GammaEvent");
	RegisterEvent<HeavyIonEvent>(HI_EVENT, "HeavyIonEvent");
}


void DragonQueue::HandleSingle(const dragon::MidasEvent& event) const
{
	switch (event.GetEventId()) {
	case DRAGON_HEAD_EVENT:
		rb::Event::Instance<GammaEvent>()->Process(&event, 0);
		break;
	case DRAGON_TAIL_EVENT:
		rb::Event::Instance<HeavyIonEvent>()->Process(&event, 0);
		break;
	default:
		err::Error("DragonQueue::HandleSingle")
			<< "Unknown event id: " << event.GetEventId() << ", skipping...\n";
		break;
	}
}

void DragonQueue::HandleCoinc(const dragon::MidasEvent& event1, const dragon::MidasEvent& event2) const
{
	dragon::CoincMidasEvent coincEvent(event1, event2);	
	rb::Event::Instance<CoincidenceEvent>()->Process(&coincEvent, 0);
}



#endif
