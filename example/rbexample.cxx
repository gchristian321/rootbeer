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
	fOffline.reset(new ifstream(filename, std::ios::binary | std::ios::in));
	return fOffline->good();
}

void ExampleBuffer::CloseFile()
{
	fOffline.reset(0);
}

Bool_t ExampleBuffer::ReadBufferOffline()
{
	return fOffline->read(fBuffer, 64*sizeof(uint32_t));
}

Bool_t ExampleBuffer::ConnectOnline(const char*, const char*, char**, int)
{
	fOnline.reset(new TRandom3());
	return kTRUE;
}

void ExampleBuffer::DisconnectOnline()
{
	fOnline.reset(0);
}

Bool_t ExampleBuffer::ReadBufferOnline()
{
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
	rb::Event::Instance<ExampleEvent>()->Process(fBuffer, 64*sizeof(uint32_t));
	return kTRUE;
}


// ====== Required Implementations ====== //

rb::Main* rb::GetMain()
{
	return new rb::Main();
}

void rb::Rint::RegisterEvents()
{
	RegisterEvent<ExampleEvent> (1, "ExampleEvent");
}

rb::BufferSource* rb::BufferSource::New()
{
	return new ExampleBuffer();
}
