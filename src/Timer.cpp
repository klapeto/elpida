/*
 * Timer.cpp
 *
 *  Created on: 21 Μαρ 2018
 *      Author: klapeto
 */

#include "Timer.hpp"
#include "Config.hpp"


#ifdef _elpida_windows
#include <Windows.h>
#endif

namespace Elpida
{

#ifdef _elpida_windows
	static inline LARGE_INTEGER getPreformanceFrequency()
	{
		LARGE_INTEGER frequency;
		QueryPerformanceFrequency(&frequency);
		return frequency;
	}
#endif

	Timer::Clock::time_point Timer::now()
	{
#ifdef _elpida_linux
		return Timer::Clock::now();
#elif _elpida_windows
		static LARGE_INTEGER frequency = getPreformanceFrequency();
		LARGE_INTEGER time;
		QueryPerformanceCounter(&time);
		time.QuadPart *= 1000000000;
		time.QuadPart /= frequency.QuadPart;
		return Clock::time_point(std::chrono::nanoseconds(time.QuadPart));
#endif
	}

} /* namespace Elpida */
