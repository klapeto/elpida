/**************************************************************************
*   Elpida - Benchmark library
*   
*   Copyright (C) 2018  Ioannis Panagiotopoulos
*   
*   This program is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*   
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*   
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <https://www.gnu.org/licenses/>
*************************************************************************/

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
