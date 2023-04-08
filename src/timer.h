#pragma once
#include <chrono>

class Timer
{
public: 
	Timer();
	~Timer();

	void update();
	double getDeltaTime();

	std::chrono::steady_clock::time_point LastUpdate;
	std::chrono::steady_clock::time_point CurrentUpdate;
};

