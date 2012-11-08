//! \file User.hxx
//! \brief Definition of user's classes.
#ifndef USER_HXX
#define USER_HXX


#if 0

// Standard includes, do not remove //
#include "Buffer.hxx"
#include "Event.hxx"
#include "Data.hxx"

#include "tstamp/TStamp.hxx"

// Definition of a BufferSource class to handle MIDAS data (at TRIUMF). //
#ifdef MIDAS_BUFFERS
#include "midas/TMidasFile.h"
#include "midas/TMidasEvent.h"
#include "dragon/MidasEvent.hxx"
#ifdef MIDAS_ONLINE
#include "midas/TMidasOnline.h"
#endif

class DragonQueue : public tstamp::Queue
{
public:
	DragonQueue(double maxDelta) : tstamp::Queue(maxDelta) { }
	~DragonQueue() { }
private:
	void HandleSingle(const dragon::MidasEvent& event1) const;
	void HandleCoinc(const dragon::MidasEvent& event1, const dragon::MidasEvent& event2) const;
};

namespace rb
{
class Midas : public rb::BufferSource
{
protected:
	Int_t fRequestId; //< Return code for online event requests.
	TMidasFile fFile; //< Offline MIDAS file.
	char fBuffer[100*1024];
	
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
	/// \todo Switch to extern "C" free functions (technically it's not allowed as-is).
	static void RunStop(int transition, int run_number, int trans_time);
	static void RunStart(int transition, int run_number, int trans_time);
	static void RunPause(int transition, int run_number, int trans_time);
	static void RunResume(int transition, int run_number, int trans_time);
	void FlushQueue();
	
};
}
#ifndef __MAKECINT__
inline rb::Midas::Midas() : fRequestId(-1) {
}
inline Bool_t rb::Midas::OpenFile(const char* file_name, char** other, int nother) {
  return fFile.Open(file_name);
}
inline void rb::Midas::CloseFile() {
  fFile.Close();
}
inline void rb::Midas::DisconnectOnline() {
#ifdef MIDAS_ONLINE
  TMidasOnline::instance()->disconnect();
	FlushQueue();
#endif
}  
inline rb::Midas::~Midas() {
  CloseFile();
  DisconnectOnline();
}
inline void rb::Midas::RunStart(int transition, int run_number, int trans_time) {
  Info("rb::Midas", "Starting run number %i.", run_number);
}
inline void rb::Midas::RunPause(int transition, int run_number, int trans_time) {
  Info("rb::Midas", "Pausing run number %i.", run_number);
}
inline void rb::Midas::RunResume(int transition, int run_number, int trans_time) {
  Info("rb::Midas", "Resuming run number %i.", run_number);
}
#endif

#include <utility>
#include "dragon/Dragon.hxx"
enum {
  DRAGON_EVENT      = 1,
  DRAGON_SCALER     = 2,
	GAMMA_EVENT       = 3,
	HI_EVENT          = 4,
	COINCIDENCE_EVENT = 5
};

class CoincideceEvent;
class GammaEvent : public rb::Event
{
private:
	rb::data::Wrapper<dragon::gamma::Gamma> fGamma;
public:
	GammaEvent();
	~GammaEvent() {}
private:
	const dragon::MidasEvent* Cast(const void* addr) {return reinterpret_cast<const dragon::MidasEvent*>(addr);}
	Bool_t DoProcess(const void* event_address, Int_t nchar);
	void HandleBadEvent() {Error("GammaEvent", "Something went wrong!!");}
	friend class CoincidenceEvent;
};

class HeavyIonEvent : public rb::Event
{
private:
	rb::data::Wrapper<dragon::hion::HeavyIon> fHeavyIon;
public:
	HeavyIonEvent();
	~HeavyIonEvent() {}
private:
	const dragon::MidasEvent* Cast(const void* addr) {return reinterpret_cast<const dragon::MidasEvent*>(addr);}
	Bool_t DoProcess(const void* event_address, Int_t nchar);
	void HandleBadEvent() {Error("HeavyIonEvent", "Something went wrong!!");}
	friend class CoincidenceEvent;
};

class CoincidenceEvent : public rb::Event
{
private:
	rb::data::Wrapper<dragon::Dragon> fDragon;
public:
	CoincidenceEvent();
	~CoincidenceEvent() {}
private:
	const dragon::CoincMidasEvent* Cast(const void* addr) {return reinterpret_cast<const dragon::CoincMidasEvent*>(addr);}
	Bool_t DoProcess(const void* event_address, Int_t nchar);
	void HandleBadEvent() {Error("CoincidenceEvent", "Something went wrong!!");}
};



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


#endif

#endif
