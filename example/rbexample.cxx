///
/// \file rbexample.cxx
/// \brief Shows a simple example of how to tie in user analysis
/// codes with ROOTBEER.
///
#include <string.h>
#include <stdint.h>
#include <TSystem.h>
#include "rbexample.hxx"



// ====== Class ExampleEvent ====== //

ExampleEvent::ExampleEvent():
	fExperiment("experiment", this, true, "")
{
	;
}

ExampleEvent::~ExampleEvent()
{
	;
}

Bool_t ExampleEvent::DoProcess(const void* pbuf, Int_t)
{
	fExperiment->Unpack(pbuf);
	return kTRUE;
}

void ExampleEvent::HandleBadEvent()
{
	std::cout << "Error processing event!!\n";
}



// ====== Class ExampleBuffer ====== //

ExampleBuffer::ExampleBuffer():
	fOnline(0), fOffline(0)
{
	;
}

ExampleBuffer::~ExampleBuffer()
{
	;
}

Bool_t ExampleBuffer::OpenFile(const char* filename, char**, int)
{
	///
	/// In the example, the offline file can just be read with a std::ifstream,
	/// so initialize it 
	fOffline.reset(new ifstream(filename, std::ios::binary | std::ios::in));
	return fOffline->good();
}

void ExampleBuffer::CloseFile()
{
	///
	/// Close the filestream
	fOffline.reset(0);
}

Bool_t ExampleBuffer::ReadBufferOffline()
{
	///
	/// These fake buffers are just 64 32-bit unsigned integers in a row,
	/// so we'll just read that amount from the file stream
	return fOffline->read(fBuffer, 64*sizeof(uint32_t));
}

Bool_t ExampleBuffer::ConnectOnline(const char*, const char*, char**, int)
{
	///
	/// We'll use a random number generator to generate fake online data. Initializing
	/// the smart pointer is analagous to actually making the connection with an online
	/// data source.
	fOnline.reset(new TRandom3());
	return kTRUE;
}

void ExampleBuffer::DisconnectOnline()
{
	///
	/// Delete the "online" random number generator
	fOnline.reset(0);
}

Bool_t ExampleBuffer::ReadBufferOnline()
{
	///
	/// Generate some fake online data with a random number generator.
	/// Each energy channel has a peak at 511 keV on top of exponential BG
	Int_t indices[32];
	uint32_t energies[32], times[32];
	for(Int_t i=0; i< 32; ++i) {
		indices[i] = i;
		energies[i] = 0;
		times[i]    = 0;
	}
	std::random_shuffle(indices, indices+32);

	Int_t ndetectors = fOnline->Integer(5) + 1;

	times[0] =  fOnline->Uniform(100, 200);
	double which = fOnline->Uniform(0,1);
	if(which > 0.85) {
		energies[indices[0]] = fOnline->Gaus(511,20);
	} else {
		energies[indices[0]] = fOnline->Exp(800);
	}

	for(Int_t i=1; i< ndetectors; ++i) {
		energies[indices[i]] = fOnline->Exp(300);
		times[indices[i]] = fOnline->Uniform(100, 200);
	}

	uint32_t both[64];
	int j=0;
	for(int i=0; i< 32; ++i) {
		both[j++] = energies[i];
		both[j++] = times[i];
	}
	memcpy(fBuffer, both, 64*sizeof(uint32_t));

	gSystem->Sleep(1);
	return kTRUE;
}

Bool_t ExampleBuffer::UnpackBuffer()
{
	///
	/// In this simple example, we only have one buffer type, so just call
	/// it's Process() function and return true;
	rb::Event::Instance<ExampleEvent>()->Process(fBuffer, 64*sizeof(uint32_t));
	return kTRUE;
}


// ====== Required Implementations ====== //

rb::Main* rb::GetMain()
{
	///
	/// We are required to implement rb::GetMain() to define how th
	/// main program should run. Just return `new rb::Main()` to use
	/// the default.
	return new rb::Main();
}

void rb::Rint::RegisterEvents()
{
	///
	/// We need to register every type of event in our experiment in
	/// this function. Each event type (here we have only one) needs
	/// a unique code (an integer) and name (a string).
	RegisterEvent<ExampleEvent> (1, "ExampleEvent");
}

rb::BufferSource* rb::BufferSource::New()
{
	///
	/// Here we tell rootbeer what type of buffer source to use
	/// by returning a new instance of the type we want - here it's
	/// `new ExampleBuffer()`
	return new ExampleBuffer();
}

const char** rb::BufferSource::GetDefaultExtensions()
{
	///
	/// Here we tell rootbeer what type of extensions to look for by 
	/// default when attaching to file via the GUI.
	static const char* ext[] = {
		"Example data file", "*dat", 
		"All files", "*.*",
		0, 0
	};

	return ext;
}
