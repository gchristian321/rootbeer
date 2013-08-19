#ifndef RB_EXAMPLE_INCLUDE_GUARD
#define RB_EXAMPLE_INCLUDE_GUARD

// C++ / ROOT includes
#include <memory>
#include <iostream>
#include <TRandom3.h>

// ROOTBEER includes
#include "Main.hxx"
#include "Data.hxx"
#include "Event.hxx"
#include "Buffer.hxx"

// Example includes
#include "Analyzer.hxx"


class ExampleBuffer: public rb::BufferSource
{
public:
	/// Initialize pointers to null
	ExampleBuffer();
	/// Empty (smart pointers used)
	virtual ~ExampleBuffer();
	/// Opens an offline file
	virtual Bool_t OpenFile(const char* file_name, char** other = 0, int nother = 0);
	/// Connects to an online experiment
	virtual Bool_t ConnectOnline(const char* host, const char* other_arg = "", char** other_args = 0, int n_others = 0);
	/// Reads event buffers from an offline file
	virtual Bool_t ReadBufferOffline();
	/// Reads event buffers from an online experiment
	virtual Bool_t ReadBufferOnline();
	/// Specifies how to deal with various received buffer types
	virtual Bool_t UnpackBuffer();
	/// Disconnects from an online experiment
	virtual void DisconnectOnline();
	/// Closes on offline file
	virtual void CloseFile();

private:
	char fBuffer[4096];
	std::auto_ptr<TRandom3> fOnline;
	std::auto_ptr<std::ifstream> fOffline;
};



/// Example event
class ExampleEvent : public rb::Event
{
private:
	/// Wrapper of Experiment class
	rb::data::Wrapper<Experiment> fExperiment;

public:
	/// Initializes fExperiment
	ExampleEvent();
	/// Empty
	~ExampleEvent();

private:
	/// Unpack the event
	Bool_t DoProcess(const void*, Int_t);
	/// What to do in case of an error in event processing
	void HandleBadEvent();
};


#endif
