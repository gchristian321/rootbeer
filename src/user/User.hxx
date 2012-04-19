//! \file User.hxx
//! \brief Definition of user's classes.
#ifndef USER_HXX
#define USER_HXX
// Standard includes, do not remove //
#include "Buffer.hxx"
#include "Event.hxx"
#include "Data.hxx"



// Definition of a BufferSource class to handle MIDAS data (at TRIUMF). //
// The macro MIDAS_BUFFERS is set in user/Makefile.user //
#ifdef MIDAS_BUFFERS
#include "midas/TMidasFile.h"
#include "midas/TMidasEvent.h"
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
#ifndef __MAKECINT__
inline rb::Midas::Midas() : fRequestId(-1) {
}
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
#endif // #ifdef __MAKECINT__

#else // #ifdef MIDAS_BUFFERS

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

#endif // #ifdef MIDAS_BUFFERS ... #else


#endif // #ifndef USER_HXX
