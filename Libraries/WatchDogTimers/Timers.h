/******************************************************************
*  Timers library for use on STACEE and CHRISTEE
*	The basic necessities for time keeping		
*
*		Brought to you by:
*              Zac Kilburn
******************************************************************/
#ifndef Timers_h
#define Timers_h
#include "Arduino.h"


class Timers
{	
public:
	Timers();
	Timers(int l);
	void resetTimer();
	void setInterval(int l);
	void updateTimer();
	bool timerDone();

private:
	int currentTime;
	int startTime;
	int length;
};
#endif
