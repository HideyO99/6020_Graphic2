#include "timer.h"

Timer::Timer()
{
}

Timer::~Timer()
{
}

void Timer::update()
{
	LastUpdate = CurrentUpdate;
	CurrentUpdate = std::chrono::steady_clock::now();
}

double Timer::getDeltaTime()
{
	std::chrono::duration<double> elapsed = CurrentUpdate - LastUpdate;
	return elapsed.count();
}

