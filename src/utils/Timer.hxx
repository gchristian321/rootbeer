/*! \file Timer.hxx
 *  \brief Defines and implements a simple timing class.
 */
#ifndef __TIMER__
#define __TIMER__
#include "TDatime.h"

namespace rb
{
/// Simple class that facilitates performing an action every \c n seconds.
/*!
 *  Example of use in a loop:
 *  \code
 *  rb::Timer myTimer(5);
 *  while(someCondition) {
 *       // Execute doSomething() every five seconds.
 *       if(!myTimer.Check()) continue;
 *       else doSomething();
 *  }
 *  myTimer.Reset(); // If we want to use again.
 *  \endcode
 */
class Timer
{
private:
	/// Internal timers, good to ~1 second.
	TDatime tstart, tstop;

	/// Timing interval.
	UInt_t fInterval;

public:

	/// Constructor
	/// Set timing interval.
	Timer(Int_t interval) {
		SetInterval(interval);
	}

	/// Set timing interval.
	void SetInterval(Int_t interval) {
		Reset();
		if(interval > 0) fInterval = interval;
		else             fInterval = 0;
	}

	/// Get timing interval.
	Int_t GetInterval() { return fInterval; }

	/// Reset the timing clock.
	void Reset() {
		tstart.Set();
		tstop.Set();
	}

	/// Check if the interval has elapsed, return \c true if yes,
	/// \c false otherwise.
	Bool_t Check() {
		if(tstop.Get() - tstart.Get() < fInterval) {
			tstop.Set();
			return kFALSE;
		}
		else {
			Reset();
			return kTRUE;
		}
	}
};
}


#endif
