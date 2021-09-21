/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2020  Ioannis Panagiotopoulos
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

#include <iomanip>
#include "Elpida/Timer.hpp"

#include "Elpida/Config.hpp"

#ifdef ELPIDA_WINDOWS
#include <windows.h>
#endif

namespace Elpida
{

#ifdef ELPIDA_WINDOWS
	static inline LARGE_INTEGER getPerformanceFrequency()
	{
		LARGE_INTEGER frequency;
		QueryPerformanceFrequency(&frequency);
		return frequency;
	}
#endif

	Timer::time_point Timer::now()
	{
#ifdef ELPIDA_LINUX
		return Timer::Clock::now();
#else
		static LARGE_INTEGER frequency = getPerformanceFrequency();
		LARGE_INTEGER time;
		QueryPerformanceCounter(&time);
		time.QuadPart *= period::den;
		time.QuadPart /= frequency.QuadPart;
		return time_point(duration(time.QuadPart));
#endif
	}

	std::string Timer::timestampToString(const Timer::time_point& time, const std::string& format)
	{
		std::time_t tt = Clock::to_time_t(time);
		std::tm tm = *std::gmtime(&tt);
		std::stringstream ss;
		ss << std::put_time(&tm, format.c_str());
		return ss.str();
	}

} /* namespace Elpida */
