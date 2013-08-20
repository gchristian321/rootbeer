///
/// \file Analyzer.cxx
/// \brief Example analysis codes for an experiment.
///
#include <string.h>
#include <stdint.h>
#include <algorithm>
#include "Analyzer.hxx"


// ====== Class Detector ====== //

Detector::Detector()
{
	Reset();
}

Detector::~Detector()
{
	;
}

void Detector::Reset()
{
	/// Set everything to zero.
	emax      = 0;
	for(int i=0; i< NUM_CHANNELS; ++i) {
		energy[i] = 0;
		time[i]   = 0;
	}
}

void Detector::Unpack(const void* addr)
{
	///
	/// Sample data format is 32 { energies, times } in order, as uint32_t.
	const uint32_t* p = reinterpret_cast<const uint32_t*>(addr);
	for(int i=0; i< NUM_CHANNELS; ++i) {
		energy[i] = *p++;
		time[i]   = *p++;
	}
	///
	/// After unpacking energies and times, calculate maximum energy
	emax = *std::max_element(energy, energy + NUM_CHANNELS);
}


// ====== Class Experiment ====== //

Experiment::Experiment()
{
	;
}

Experiment::~Experiment()
{
	;
}

void Experiment::Unpack(const void* pbuffer)
{
	Reset();
	detector.Unpack(pbuffer);
}

void Experiment::Reset()
{
	detector.Reset();
}
