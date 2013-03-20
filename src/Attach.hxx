//! \file Attach.hxx
//! \brief Defines classes for attaching to various data sources.
#ifndef RB_ATTACH_HEADER
#define RB_ATTACH_HEADER
#include "utils/boost_scoped_ptr.h"
#include "utils/Timer.hxx"
#include "Buffer.hxx"


namespace rb
{

//! Specialized timer class for reading generic data
template <class T>
class AttachTimer: public rb::Timer
{
private:
	boost::scoped_ptr<T> fAttach;
public:
	AttachTimer(Long_t timeout, T* attach):
		rb::Timer(timeout),	fAttach(attach) { }
	void DoAction() { fAttach->TimerAction(); }
	virtual void TurnOff()
		{
			TTimer::TurnOff();
			fAttach.reset(0);
		}
	ClassDef(AttachTimer, 0);
};


// \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// \\\\\\\\\\\\  FILE  \\\\\\\\\\\\//
// \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

//! Class for attaching to offline files
class FileAttach
{
private:
	//! Timeout time
	Long_t fTimeout;
	//! Pointer to TTimer object
	boost::scoped_ptr<TTimer> fTimer;
	//! Pointer to a BufferSource derived class used for getting and unpacking buffers.
	boost::scoped_ptr<BufferSource> fBuffer;
	//! Name (path) of the offline file.
	std::string kFileName;
	//! Tells whether to stop reading at EOF (true) or stay connected and wait for more data to come in (false).
	const Bool_t kStopAtEnd;
	//! Buffer counter
	Long_t fNbuffers;

public:
	//! \details Take care of EOF cleanup
	virtual ~FileAttach();
	//! \brief Open the file, loop contents and use fBuffer to extract and unpack data.
	void TimerAction();
		//! \brief Conststructs a \c new instance of rb::FileAttach and calls StartLoop()
	static void Go(const char* filename, Bool_t stopAtEnd);
	//! \brief Stop timer and end attachment
	static void Stop();

private:
	//! \brief Set kFileName and kStopAtEnd, initialize fBuffer to the result
	//! of BufferSource::New()
	FileAttach(const char* filename, Bool_t stopAtEnd);
	//! Start running the loop
	void StartLoop();
};

inline void rb::FileAttach::StartLoop() {
	fTimer.reset(new AttachTimer<FileAttach>(fTimeout, this));
	fTimer->Start();
}

inline void FileAttach::Go(const char* filename, Bool_t stopAtEnd) {
	FileAttach * f = new FileAttach(filename, stopAtEnd);
	f->StartLoop();
}


// // \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// // \\\\\\\\\\\\ ONLINE \\\\\\\\\\\\//
// // \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

//! Class for attaching to online data
class OnlineAttach
{
private:
	//! Timeout time
	Long_t fTimeout;
	//! Pointer to TTimer object
	boost::scoped_ptr<TTimer> fTimer;
	//! \brief The source of the online data.
	//! \details Usually this is the host name or ip address of where the data come in from.
	char* fSourceArg;
	//! \brief Secondary argument that might be needed to obtain online data.
	//! \details Examples: MIDAS experiment name, port number, etc.
	char* fOtherArg;
	//! \brief Any other arguments that are not covered by the first two.
	//! \details Like the arguments to main (<tt>char** argc</tt>), this can basically be used
	//! to represent anything.
	char** fOtherArgs;
	//! The length of the \c char* array fOtherArgs.
	const int fNumOthers;
	//! Pointer to a BufferSource derived class used for getting and unpacking buffers.
	boost::scoped_ptr<BufferSource> fBuffer;
	//! Buffer counter
	Long_t fNbuffers;

public:
	//! Take care of EOR cleanup
	virtual ~OnlineAttach();
	//! \brief Make connection to online source and look for data
	void TimerAction();
	//! \brief Conststructs a \c new instance of rb::OnlineAttach and calls StartLoop()
	static void Go(const char* source, const char* other, char** others, int nothers);
	//! \brief Stop timer and end attachment
	static void Stop();

private:
	//! \brief Initialize
	OnlineAttach(const char* source, const char* other, char** others, int nothers);
	//! Start running the loop
	void StartLoop();
};

inline void rb::OnlineAttach::StartLoop() {
	fTimer.reset(new rb::AttachTimer<rb::OnlineAttach>(fTimeout, this));
	fTimer->Start();
}

inline void OnlineAttach::Go(const char* source, const char* other, char** others, int nothers) {
	OnlineAttach * o = new OnlineAttach(source, other, others, nothers);
	o->StartLoop();
}

Bool_t FileAttached();
Bool_t OnlineAttached();

} // namespace rb


#ifdef __MAKECINT__
#pragma link C++ class rb::AttachTimer<rb::FileAttach>+;
#pragma link C++ class rb::AttachTimer<rb::OnlineAttach>+;
#endif


#endif
