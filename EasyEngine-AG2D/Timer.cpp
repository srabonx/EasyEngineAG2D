#include "Timer.h"

namespace EE {
	Timer::Timer(void) {
		timer_start = timeGetTime();
		reset();
	}

	Timer::~Timer(void)
	{
	}

	DWORD Timer::getTimer()
	{
		return (DWORD)(timeGetTime());
	}

	DWORD Timer::getStartTimeMillis()
	{
		return (DWORD)(timeGetTime() - timer_start);
	}

	void Timer::sleep(int ms)
	{
		DWORD start = getTimer();
		while (start + ms > getTimer());				// Just eats CPU cycle LOL
	}

	void Timer::reset()
	{
		stopwatch_start = getTimer();
	}

	bool Timer::stopwatch(int ms)
	{
		if (timeGetTime() > stopwatch_start + ms) {
			stopwatch_start = getTimer();
			return true;
		}

		return false;
	}

};