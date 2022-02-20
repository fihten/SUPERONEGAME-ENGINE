#pragma once

#include <windows.h>

class Timer
{
	Timer();
public:
	~Timer();
	static Timer* Instance();
	float elapsedTime();
private:
	static Timer* timer;
	static float time;
};


