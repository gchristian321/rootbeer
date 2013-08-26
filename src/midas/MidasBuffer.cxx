/// \file MidasBuffer.cxx
/// \author G. Christian
/// \brief Implements MidasBuffer.hxx
#include <cassert>
#include "TMidasFile.h"
#include "TMidasEvent.h"
#include "Attach.hxx"
#include "MidasBuffer.hxx"

#ifdef MIDASSYS
#include "midas.h"
#endif



rb::MidasBuffer* rb::MidasBuffer::fgInstance = 0;


rb::MidasBuffer::MidasBuffer(ULong_t size, Int_t trpStart, Int_t trpStop, Int_t trpPause, Int_t trpResume):
	fIsConnected(false),
	fBufferSize(size),
	fIsTruncated(false),
	fFile(0),
	fType(MidasBuffer::NONE)
{
	/*!
	 * \param size Size of the internal buffer in bytes. This should be larger than the
	 * biggest expected event.
	 */
	assert(fgInstance == 0);
	try {
		fBuffer = new Char_t[size];
	} catch (std::bad_alloc& e) { ///\todo Better (non-fatal) error handling for bad alloc.
		err::Error("rb::Midas::Buffer::MidasBuffer") << "Couldn't allocate memory!";
		throw (e);
	}

	SetTransitionPriorities(trpStart, trpStop, trpPause, trpResume);
}

rb::MidasBuffer::~MidasBuffer()
{
	if(fgInstance) {
		delete[] fBuffer;
		fgInstance = 0;
		if(fFile) {
			TMidasFile* pFile = (TMidasFile*)fFile;
			delete pFile;
			fFile = 0;
		}
	}
}

Bool_t rb::MidasBuffer::ReadBufferOffline()
{
	/*!
	 * Reads event data into fBuffer
	 * \todo Could be made more efficient (no copy)??
	 */
	assert(fFile);
	rb::TMidasEvent temp;
	TMidasFile* pFile = (TMidasFile*)fFile;
	Bool_t have_event = pFile->Read(&temp);

	if(have_event) {
		memcpy (fBuffer, temp.GetEventHeader(), sizeof(rb::TMidas_EVENT_HEADER));
		memcpy (fBuffer + sizeof(rb::TMidas_EVENT_HEADER), temp.GetData(), temp.GetDataSize());

		if ( temp.GetDataSize() + sizeof(rb::TMidas_EVENT_HEADER) > fBufferSize ) {
			err::Warning("rb::MidasBuffer::ReadBufferOffline")
				<< "Received a truncated event: event size = "
				<< ( temp.GetDataSize() + sizeof(rb::TMidas_EVENT_HEADER) )
				<< ", max size = " << fBufferSize << " (Id, serial = "
				<< temp.GetEventId() << ", " << temp.GetSerialNumber() << ")";
			fIsTruncated = true;
		}
	}

	return have_event;
}

Bool_t rb::MidasBuffer::UnpackBuffer()
{
	/*!
	 * Compose a TMidasEvent and then let the user handle it.
	 */
	rb::TMidas_EVENT_HEADER* pHeader = reinterpret_cast<rb::TMidas_EVENT_HEADER*>(fBuffer);
	char* pEvent = fBuffer + sizeof(rb::TMidas_EVENT_HEADER);
	return UnpackEvent(pHeader, pEvent);
}

Bool_t rb::MidasBuffer::OpenFile(const char* file_name, char** other, int nother)
{
	/*!
	 * Open MIDAS file w/ TMidasFile::Open(), call run start transition handler.
	 */
	fType = MidasBuffer::OFFLINE;
	RunStartTransition(0);
	TMidasFile* f = new TMidasFile();
	bool status = f->Open(file_name);
	if (status == kTRUE) {
		fFile = f;
	}
	else {
		delete f;
		fType = MidasBuffer::NONE;
	}
	return status;
}

void rb::MidasBuffer::CloseFile()
{
	/*! Close file, do run stop transition */
	TMidasFile* pFile = (TMidasFile*)fFile;
	err::Info("rb::MidasBuffer::CloseFile")
		<< "Closing MIDAS file: \"" << pFile->GetFilename() << "\"";
	pFile->Close();
	delete pFile;
	fFile = 0;

	RunStopTransition(0);
	fType = MidasBuffer::NONE;
}

void rb::MidasBuffer::SetTransitionPriorities(Int_t prStart, Int_t prStop,
																							Int_t prPause, Int_t prResume)
{
	fTransitionPriorities[0] = prStart;
	fTransitionPriorities[1] = prStop;
	fTransitionPriorities[2] = prPause;
	fTransitionPriorities[3] = prResume;
}


// ================ ONLINE ============ //
#ifdef MIDASSYS

#define M_ONLINE_BAIL_OUT \
	fType = MidasBuffer::NONE; cm_disconnect_experiment(); fIsConnected = false; return false

Bool_t rb::MidasBuffer::ConnectOnline(const char* host, const char* experiment, char**, int)
{
	/*!
	 * \param host hostname:port where the experiment is running (e.g. ladd06:7071)
	 * \param experiment Experiment name on \e host (e.g. "dragon")
	 *
	 * See list below for what's specifically handled by this function.
	 */
	INT status;
	char systembuf[] = "SYSTEM";
	fType = MidasBuffer::ONLINE;

	/// - Connect to MIDAS experiment
	status = cm_connect_experiment (host, experiment, "rootbeer", NULL);
	if (status != CM_SUCCESS) {
		err::Error("rb::MidasBuffer::ConnectOnline")
			<< "Couldn't connect to experiment \"" << experiment << "\" on host \""
			<<  host << "\", status = " << status;
		return false;
	}

	fIsConnected = true;
	err::Info("rb::MidasBuffer::ConnectOnline")
		<< "Connected to experiment \"" << experiment << "\" on host \"" << host;

	/// - Get database handle
	status = cm_get_experiment_database(&fDb, 0);
	if (status != CM_SUCCESS) {
		err::Error("rb::MidasBuffer::ConnectOnline")
			<< "Couldn't read experiment database";
		M_ONLINE_BAIL_OUT;
	}

	/// - Connect to "SYSTEM" shared memory buffer
  status = bm_open_buffer(systembuf, 2*MAX_EVENT_SIZE, &fBufferHandle);
	if (status != CM_SUCCESS) {
		err::Error("rb::MidasBuffer::ConnectOnline")
			<< "Error opening \"" << systembuf << "\" shared memory buffer, status = "
			<< status;
		M_ONLINE_BAIL_OUT;
	}

	/// - Request (nonblocking) all types of events from the "SYSTEM" buffer
	status = bm_request_event(fBufferHandle, -1, -1, GET_NONBLOCKING, &fRequestId, NULL);
	if (status != CM_SUCCESS) {
		err::Error("rb::MidasBuffer::ConnectOnline")
			<< "Error requesting events from \"" << systembuf << "\", status = "
			<< status;
		M_ONLINE_BAIL_OUT;
	}

	/// - Register transition handlers
	/// \note Stop transition needs to have a 'late' (>700) priority to receive
	///  events flushed from the "SYSTEM" buffer at the end of the run
	cm_register_transition(TR_START,  rb_run_start,  fTransitionPriorities[0]);
	cm_register_transition(TR_STOP,   rb_run_stop,   fTransitionPriorities[1]);
	cm_register_transition(TR_PAUSE,  rb_run_pause,  fTransitionPriorities[2]);
	cm_register_transition(TR_RESUME, rb_run_resume, fTransitionPriorities[3]);

	/// - Call run start transition handler if any of the following are true:
	///    -# State is running
	///    -# State is paused
	///    -# Transition is in progress
	Int_t runstate, transition, isize = sizeof(Int_t);
	if(1)
		status = db_get_value (fDb, 0, "/Runinfo/State", &runstate, &isize, TID_INT, false);
	if(status == CM_SUCCESS)
		status = db_get_value (fDb, 0, "/Runinfo/Transition in progress", &transition, &isize, TID_INT, false);

	if(status == CM_SUCCESS) {
		if (runstate == STATE_RUNNING || runstate == STATE_PAUSED || transition != 0) {
			Int_t runnumber, isize = sizeof(Int_t);
			status = db_get_value (fDb, 0, "/Runinfo/Run number", &runnumber, &isize, TID_INT, false);
			if(status == CM_SUCCESS)
				RunStartTransition(runnumber);
		}
	}
	if(status != CM_SUCCESS)
		cm_msg(MERROR, "rootbeer", "Error reading from ODB, status = %d", status);

	return true;
}
		
void rb::MidasBuffer::DisconnectOnline()
{
	/*! Calls cm_disconnect_experiment() and run stop handler */
	Int_t runnumber, isize = sizeof(Int_t), status;
	status = db_get_value (fDb, 0, "/Runinfo/Run number",
												 &runnumber, &isize, TID_INT, false);
	cm_disconnect_experiment();
	fIsConnected = false;
	if(status == CM_SUCCESS)
		RunStopTransition(runnumber);
	fType = MidasBuffer::NONE;
	err::Info("rb::MidasBuffer::DisconnectOnline")
		<< "Disconnecting from experiment";
}

Bool_t rb::MidasBuffer::ReadBufferOnline()
{
	/*!
	 * Uses bm_receive_event to directly receive events from "SYSTEM" shared memory. The function
	 * requests events in a loop until it either gets one or receives a signal to exit.
	 *
	 * See the list below for what is done in the request loop.
	 */
	bool have_event = false;
	const int timeout = 0;
	INT size = fBufferSize, status;

	/// - Check status of client w/ cm_yield()
	status = cm_yield(timeout);

	cm_set_watchdog_params(TRUE,  60*1000);
	cm_watchdog(0);
	cm_set_watchdog_params(FALSE, 60*1000);

	/// - Then check for an event
	if (status != RPC_SHUTDOWN) status = bm_receive_event (fBufferHandle, fBuffer, &size, ASYNC);

	/// - If we have an event (full or partial), return true
	if (status == BM_SUCCESS || status == BM_TRUNCATED) {
		///  - Print a warning message if the event was truncated
		if (status == BM_TRUNCATED) {
			err::Warning("rb::MidasBuffer::ReadBufferOnline")
				<< "Received a truncated event: event size = "
				<< ( reinterpret_cast<rb::TMidas_EVENT_HEADER*>(fBuffer)->fDataSize + sizeof(rb::TMidas_EVENT_HEADER) )
				<< ", max size = " << fBufferSize;
			fIsTruncated = true;
		}
		return true;
	}

	/// - Print an error message if the buffer handle was invalid, and unattach
	if (status == BM_INVALID_HANDLE) {
		err::Error("rb::MidasBuffer::ReadBufferOnline") << "Invalid buffer handle: " << fBufferHandle;
		rb::OnlineAttach::Stop();
	}

	/// - If we received a shutdown command from MIDAS, stop the online loop timer
	if (status == RPC_SHUTDOWN || status == SS_ABORT) {
		const char* cmd = status == RPC_SHUTDOWN ? "RPC_SHUTDOWN" : "SS_ABORT";
		err::Info("rb::MidasBuffer::ReadBufferOnline")
			<< "Received MIDAS command: " << cmd << ", unattaching from online data.";
		rb::OnlineAttach::Stop();
	}

	/// - Otherwise, return false (no event, keep looking).
	return false;
}

#else // #ifdef MIDASSYS

#define M_NO_MIDASSYS(FUNC) do {																				\
		err::Error(FUNC) <<																									\
			"Online functionality requires MIDAS to be installed on your system."; \
	} while (0)

Bool_t rb::MidasBuffer::ConnectOnline(const char* host, const char* experiment, char**, int)
{
	M_NO_MIDASSYS("rb::MidasBuffer::ConnectOnline");
	return false;
}

void rb::MidasBuffer::DisconnectOnline()
{
	M_NO_MIDASSYS("rb::MidasBuffer::DisconnectOnline");
}

Bool_t rb::MidasBuffer::ReadBufferOnline()
{
	M_NO_MIDASSYS("rb::MidasBuffer::ReadBufferOnline");
	return false;
}

#endif

rb::MidasBuffer* rb::MidasBuffer::Instance()
{
	if(!fgInstance)
		fgInstance = rb::MidasBuffer::Create();
	return fgInstance;
}

rb::BufferSource* rb::BufferSource::New()
{
	return rb::MidasBuffer::Instance();
}

void rb::MidasBuffer::RunStopTransition(Int_t runnum)
{
	if(runnum > 0)
		err::Info("rb::MidasBuffer") << "Stopping run " << runnum;
}

void rb::MidasBuffer::RunStartTransition(Int_t runnum)
{
	if(runnum > 0)
		err::Info("rb::MidasBuffer") << "Starting run " << runnum;
}

void rb::MidasBuffer::RunPauseTransition(Int_t runnum)
{
	if(runnum > 0)
		err::Info("rb::MidasBuffer") << "Pausing run " << runnum;
}

void rb::MidasBuffer::RunResumeTransition(Int_t runnum)
{
	if(runnum > 0)
		err::Info("rb::MidasBuffer") << "Resuming run " << runnum;
}


#ifndef MIDASSYS
#define CM_SUCCESS 1
#endif

Int_t rb_run_stop(Int_t runnum, char* err)
{
	bm_empty_buffers();
	rb::MidasBuffer::Instance()->RunStopTransition(runnum);
	return CM_SUCCESS;
}

Int_t rb_run_start(Int_t runnum, char* err)
{
	rb::MidasBuffer::Instance()->RunStartTransition(runnum);
	return CM_SUCCESS;
}

Int_t rb_run_pause(Int_t runnum, char* err)
{
	rb::MidasBuffer::Instance()->RunPauseTransition(runnum);
	return CM_SUCCESS;
}

Int_t rb_run_resume(Int_t runnum, char* err)
{
	rb::MidasBuffer::Instance()->RunResumeTransition(runnum);
	return CM_SUCCESS;
}
