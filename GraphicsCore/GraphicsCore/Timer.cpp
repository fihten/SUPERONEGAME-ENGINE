#include "Timer.h"

Timer* Timer::timer = 0;
float Timer::time = 0;

Timer::Timer()
{
	__int64 count = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)& count);

	__int64 countsPerSec = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)& countsPerSec);

	double secsPerCount = 1.0 / (double)countsPerSec;

	time = count * secsPerCount;
}

Timer::~Timer()
{
	timer = 0;
	time = 0;
}

Timer* Timer::Instance()
{
	if (timer == 0)
		timer = new Timer();
	return timer;
}

float Timer::elapsedTime()
{
	__int64 count = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)& count);

	__int64 countsPerSec = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)& countsPerSec);

	float newTime = (double)count / (double)countsPerSec;
	
	float result = newTime > time ? newTime - time : 0.0f;

	time = newTime;

	return result;
}

