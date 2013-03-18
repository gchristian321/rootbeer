/// \file MidasBuffer.hxx
/// \author G. Christian
/// \brief Generic implementation of rb::BufferSource for MIDAS experiments.
#ifndef DRAGON_RB_MIDASBUFFER_HXX
#define DRAGON_RB_MIDASBUFFER_HXX
#include "TMidasFile.h"
#include "TMidasEvent.h"
#include "Buffer.hxx"

#ifdef MIDASSYS
#include <midas.h>
#endif 


namespace rb {

class MidasBuffer: public rb::BufferSource
{
public:
	enum Etype { ONLINE, OFFLINE, NONE };

private:
	/// Singleton instance
	static MidasBuffer* fgInstance;

protected:
	/// Return code for online event requests.
	Int_t fRequestId;

	/// MIDAS shared memory buffer handle
	Int_t fBufferHandle;

	/// MIDAS database handle
	Int_t fDb;

  /// Storage buffer for events
	Char_t* fBuffer;

	/// Size of the storage buffer
	ULong_t fBufferSize;

	/// Flag for truncated MIDAS events
	Bool_t fIsTruncated;

	/// Transition handler priorities
	Int_t fTransitionPriorities[4];
	
	/// Offline MIDAS file.
	rb::TMidasFile fFile;

	/// Type code (online or offline)
	Int_t fType;

protected:
	/// Sets fIsTruncated to false, and allocates the internal buffer
	MidasBuffer(ULong_t size = 1024*1024);

	/// Frees fBuffer
	virtual ~MidasBuffer();

public:
	/// Opens an offline MIDAS file
	virtual Bool_t OpenFile(const char* file_name, char** other = 0, int nother = 0);

	/// Connects to an online MIDAS experiment
	virtual Bool_t ConnectOnline(const char* host, const char* other_arg = "", char** other_args = 0, int n_others = 0);

	/// Reads event buffers from an offline MIDAS file
	virtual Bool_t ReadBufferOffline();

	/// Reads event buffers from an online MIDAS experiment
	virtual Bool_t ReadBufferOnline();

	/// Specifies how to deal with various received buffer types
	virtual Bool_t UnpackBuffer();

	/// Disconnects from an online MIDAS experiment
	virtual void DisconnectOnline();

	/// Closes on offline MIDAS file
	virtual void CloseFile();

public:
	/// Pure virtual function to unpack a midas event
	virtual Bool_t UnpackEvent(TMidasEvent*) = 0;

	/// Virtual run start transition handler
	virtual void RunStartTransition(Int_t runnum);

	/// Virtual run stop transition handler
	virtual void RunStopTransition(Int_t runnum);

	/// Virtual run pause transition handler
	virtual void RunPauseTransition(Int_t runnum);

	/// Virtual run resume transition handler
	virtual void RunResumeTransition(Int_t runnum);

	/// Set transition handler priorities
	void SetTransitionPriorities(Int_t prStart, Int_t prStop, Int_t prPause, Int_t prResume);

	/// Singleton instance
	static MidasBuffer* Instance();

	/// User defined static method to return his/her MidasBuffer class
	static MidasBuffer* Create();

private:
	/// Disallow copy
	MidasBuffer(const MidasBuffer&) {  }

	/// Disallow assign
	MidasBuffer& operator= (const MidasBuffer&) { return *this; }
};

} // namespace rb


extern "C" {

/// Run stop transition handler
Int_t rb_run_stop(Int_t runnum, char* err);
/// Run start transition handler
Int_t rb_run_start(Int_t runnum, char* err);
/// Run pause transition handler
Int_t rb_run_pause(Int_t runnum, char* err);
/// Run resume transition handler
Int_t rb_run_resume(Int_t runnum, char* err);

}


#endif
