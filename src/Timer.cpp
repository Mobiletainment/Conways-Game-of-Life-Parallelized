//////////////////////////////////////////////////////////////////////////////
// Timer.cpp
// =========
// High Resolution Timer.
// This timer is able to measure the elapsed time with 1 micro-second accuracy
// in both Windows, Linux and Unix system 
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2003-01-13
// UPDATED: 2006-01-13
//
// Copyright (c) 2003 Song Ho Ahn
//////////////////////////////////////////////////////////////////////////////

#include "Timer.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string>
using namespace std;

///////////////////////////////////////////////////////////////////////////////
// constructor
///////////////////////////////////////////////////////////////////////////////
Timer::Timer()
{
#ifdef WIN32
	QueryPerformanceFrequency(&frequency);
	startCount.QuadPart = 0;
	endCount.QuadPart = 0;
#else
	startCount.tv_sec = startCount.tv_usec = 0;
	endCount.tv_sec = endCount.tv_usec = 0;
#endif

	stopped = 0;
	startTimeInMicroSec = 0;
	endTimeInMicroSec = 0;
}



///////////////////////////////////////////////////////////////////////////////
// distructor
///////////////////////////////////////////////////////////////////////////////
Timer::~Timer()
{
}



///////////////////////////////////////////////////////////////////////////////
// start timer.
// startCount will be set at this point.
///////////////////////////////////////////////////////////////////////////////
void Timer::start()
{
	stopped = 0; // reset stop flag
#ifdef WIN32
	QueryPerformanceCounter(&startCount);
#else
	gettimeofday(&startCount, NULL);
#endif
}



///////////////////////////////////////////////////////////////////////////////
// stop the timer.
// endCount will be set at this point.
///////////////////////////////////////////////////////////////////////////////
void Timer::stop()
{
	stopped = 1; // set timer stopped flag

#ifdef WIN32
	QueryPerformanceCounter(&endCount);
#else
	gettimeofday(&endCount, NULL);
#endif
}



///////////////////////////////////////////////////////////////////////////////
// compute elapsed time in micro-second resolution.
// other getElapsedTime will call this first, then convert to correspond resolution.
///////////////////////////////////////////////////////////////////////////////
double Timer::getElapsedTimeInMicroSec()
{
#ifdef WIN32
	if(!stopped)
		QueryPerformanceCounter(&endCount);

	startTimeInMicroSec = startCount.QuadPart * (1000000.0 / frequency.QuadPart);
	endTimeInMicroSec = endCount.QuadPart * (1000000.0 / frequency.QuadPart);
#else
	if(!stopped)
		gettimeofday(&endCount, NULL);

	startTimeInMicroSec = (startCount.tv_sec * 1000000.0) + startCount.tv_usec;
	endTimeInMicroSec = (endCount.tv_sec * 1000000.0) + endCount.tv_usec;
#endif

	return endTimeInMicroSec - startTimeInMicroSec;
}



///////////////////////////////////////////////////////////////////////////////
// divide elapsedTimeInMicroSec by 1000
///////////////////////////////////////////////////////////////////////////////
double Timer::getElapsedTimeInMilliSec()
{
	return this->getElapsedTimeInMicroSec() * 0.001;
}



///////////////////////////////////////////////////////////////////////////////
// divide elapsedTimeInMicroSec by 1000000
///////////////////////////////////////////////////////////////////////////////
double Timer::getElapsedTimeInSec()
{
	return this->getElapsedTimeInMicroSec() * 0.000001;
}



///////////////////////////////////////////////////////////////////////////////
// same as getElapsedTimeInSec()
///////////////////////////////////////////////////////////////////////////////
double Timer::getElapsedTime()
{
	return this->getElapsedTimeInSec();
}

const string Timer::getElapsedTimeString()
{
	char time[16];
	double elapsedTime = this->getElapsedTimeInMilliSec();

	if (elapsedTime < 1000)
	{
		sprintf_s(time, "00:00:00.%03.0lf", elapsedTime); 	
	}
	else if (elapsedTime < 60000)
	{
		int seconds = (int)(elapsedTime * 0.001);
		sprintf_s(time, "00:00:%02d.%03.0lf", seconds, elapsedTime - (seconds * 1000)); 
	}
	else
	{	
		double hoursD = elapsedTime / 3600000;
		int hours = (int)hoursD;
		int minutes = (int) (hoursD - hours) * 60;
		double secondsD;
		double milliseconds = modf(elapsedTime * 0.001, &secondsD) * 1000; //, &milliseconds);
		int seconds = (int) (secondsD) % 60 ;

		sprintf_s(time, "%02d:%02d:%02d.%03.0lf", hours, minutes, seconds, milliseconds); 
	}

	string timeString = time;
	return timeString;
}