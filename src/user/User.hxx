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
/// \brief BufferSource class for the MIDAS system
class Midas : public rb::BufferSource
{
protected:
	 /// Return code for online event requests.
	 Int_t fRequestId;
	 /// Offline MIDAS file.
	 TMidasFile fFile;
	 /// Midas event buffer.
	 TMidasEvent fBuffer;
public:
	 /// Constructor, just sets fRequestId to -1
	 Midas();

	 /// Destructor, close any files and disconnect from any online sources
	 virtual ~Midas();

	 /// \brief Open an offline MIDAS file
	 /// \param [in] file_name Path of the file to be opened
	 /// \param other Unused
	 /// \param nother Unused
	 virtual Bool_t OpenFile(const char* file_name, char** other = 0, int nother = 0);

	 /// \brief Connect to an online MIDAS instance
	 /// \param [in] host Name of the host on which MIDAS is running
	 /// \param [in] expt MIDAS Experiment name on the host
	 /// \param other_args Unused
	 /// \param n_others Unused
   /// \bug Program crashes when trying to connect online more than once in given session. This seems to be
   /// due to defficiencies in MIDAS itself, namely that it only allows clients to connect once during the
   /// course of an executable. Something about the way in which \c cm_connect_experiment allocates and
   /// \c cm_disconnect_experiment de-allocates resources causes the connect...disconnect cycle to only be possible
   /// once in an executable. Likely this is because of the use of application scope global variables in midas.
	 virtual Bool_t ConnectOnline(const char* host, const char* expt = "", char** other_args = 0, int n_others = 0);

	 /// \brief Receive a MIDAS buffer from an online source
	 virtual Bool_t ReadBufferOffline();

	 /// \brief Receive a MIDAS buffer froman offline source
	 virtual Bool_t ReadBufferOnline();

	 /// \brief Unpack a midas buffer
	 /// \note The implementation of this function depends entirely on the user's experiment; hence it
	 /// is not implemented in stock rootbeer.
	 virtual Bool_t UnpackBuffer();

	 /// \brief Close out a connection to a MIDAS file
	 virtual void CloseFile();

	 /// \brief Disconnect from an online MIDAS instance
	 virtual void DisconnectOnline();

protected:
	 /// What to to upon starting a new run when connected online
	 static void RunStart(int transition, int run_number, int trans_time) {
		 Info("rb::Midas", "Starting run number %i.", run_number);
	 }
	 /// What to to upon stopping a run when connected online
	 static void RunStop(int transition, int run_number, int trans_time) {
		 Info("rb::Midas", "Stopping run number %i.", run_number);
	 }
	 /// What to to upon pausing a run when connected online
	 static void RunPause(int transition, int run_number, int trans_time) {
		 Info("rb::Midas", "Pausing run number %i.", run_number);
	 }
	 /// What to to upon resuming a paused run when connected online
	 static void RunResume(int transition, int run_number, int trans_time) {
		 Info("rb::Midas", "Resuming run number %i.", run_number);
	 }
};
}

#else // #ifdef MIDAS_BUFFERS

// Throw a compile-time error.  The user should remove this one he/she has done what's required.
#error "You need to define a derived class of rb::BufferSource and implement rb::BufferSource::New()."

// Here's what you need:
//
// class /* ClassName */ : public rb::BufferSource()
// {
//   protected:
//   // Data, etc.
//   public:
//   // Required functions //
// 	/*Name*/ (); // Constructor
// 	virtual ~/*Name*/ (); // Destructor
// 	virtual Bool_t OpenFile(const char* file_name, char** other = 0, int nother = 0);
// 	virtual Bool_t ConnectOnline(const char* host, const char* other_arg = "", char** other_args = 0, int n_others = 0);
// 	virtual Bool_t ReadBufferOffline();
// 	virtual Bool_t ReadBufferOnline();
// 	virtual Bool_t UnpackBuffer();
// 	virtual void CloseFile();
// 	virtual void DisconnectOnline();
//   protected:
// 	// Internal functions, etc.
// };

#endif // #ifdef MIDAS_BUFFERS ... #else


#endif // #ifndef USER_HXX
