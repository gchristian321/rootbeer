#ifndef ROOTBEER_ANALYZER_ANALYZER_HXX
#define ROOTBEER_ANALYZER_ANALYZER_HXX

static const int NUM_CHANNELS = 32;

/// Sample Class for a detector
class Detector {
public: // Methods
	/// Calls Reset();
	Detector();
	/// Empty
	~Detector();
	/// Unpack event data into class members
	void Unpack(const void* addr);
	/// Reset data after an event
	void Reset();
public: // Data
	/// Energies for a 32-channel array of detectors
	double energy[NUM_CHANNELS];
	/// Times for a 32-channel array of detectors
	double time[NUM_CHANNELS];
	/// Highest energy in the array of 32 detectors
	double emax;
};

/// Example analysis class for an experiment
class Experiment {

public: // Methods
	/// Empy
	Experiment();
	/// Empty
	~Experiment();
	/// Unpacks data into class structure
	void Unpack(const void* pbuffer);
	/// Resets all data to defaults
	void Reset();
public: // Data
	/// Instance of detector class
	Detector detector;
};


#endif
