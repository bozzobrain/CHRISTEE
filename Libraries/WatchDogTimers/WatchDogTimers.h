/******************************************************************
*  Timers library for use on CHRISTEE
*	The basic necessities for time keeping
*
*		Brought to you by:
*              Zac Kilburn and Todd Locker
******************************************************************/
#ifndef WatchDogTimers_h
#define WatchDogTimers_h
#include "Arduino.h"


class Timers
{
public:
	Timers();
	Timers(long l);
	void resetTimer();
	void setInterval(long l);
	void updateTimer();
	bool timerDone();

private:
	long currentTime;
	long startTime;
	long length;
};
#endif
