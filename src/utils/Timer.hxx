/*! \file Timer.hxx
 *  \brief Defines and implements a simple timing class.
 */
#ifndef RB_TIMER_HEADER
#define RB_TIMER_HEADER
#include <TTimer.h>
#include <TTimeStamp.h>

namespace rb
{
//! Simple class to facilitate time difference calculations.
class Time
{
private:
	//! Internal timing object
	TTimeStamp fTimestamp;
public:
	//! Initialize fTimestamp to the current time.
	Time() : fTimestamp() { }
	//! Calculate the difference between two times
	Double_t operator- (const Time& lhs) const
		{
			//! \returns Time difference in seconds
			return fTimestamp.AsDouble() - lhs.fTimestamp.AsDouble();
		}
};

//! Class to check if an elapsed time has passed
class Timeout
{
private:
	//! Time at start
	rb::Time fStart;
	//! Desired timeout in msec.
	Long_t fTimeout;
public:
	//! Initialize start time, desired timeout
	Timeout(Long_t timeout):
		fStart(), fTimeout(timeout)
		{
			//! \param timeout Desired timeout in milliseconds.
		}
	//! Check if the timeout has elapsed
	Bool_t Check()
		{
			//! \returns True if the timeout has elapsed, false if not
			rb::Time tnow;
			return 1e3*(tnow - fStart) < fTimeout ? kFALSE : kTRUE;
		}
};

//! \brief Timer class performing a generic action every \e n milliseconds
class Timer: public TTimer
{
public:
	//! Sets default timeout, action
	Timer(Long_t timeout):
		TTimer(timeout, kTRUE)
		{
			//! \param timeout Default timeout in milliseconds.
		}
	//! Empty
	virtual ~Timer() { }
	//! Perform the desired action, reset the timer.
	Bool_t Notify() {
		DoAction();
		Reset();
		return kTRUE;
	}
private:
	//! Performs the desired action every timeout period
	virtual void DoAction() = 0;
};

} // namespace rb



#endif
