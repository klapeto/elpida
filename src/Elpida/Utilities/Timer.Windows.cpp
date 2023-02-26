/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2023  Ioannis Panagiotopoulos
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

//
// Created by klapeto on 25/2/2023.
//

#include "Elpida/Config.hpp"

#if defined(ELPIDA_WINDOWS)

#include "Elpida/Timer.hpp"
#include <windows.h>

namespace Elpida
{

	static inline LARGE_INTEGER getPerformanceFrequency()
	{
		LARGE_INTEGER frequency;
		QueryPerformanceFrequency(&frequency);
		return frequency;
	}

	Timer::time_point Timer::now()
	{
		static LARGE_INTEGER frequency = getPerformanceFrequency();
		LARGE_INTEGER time;
		QueryPerformanceCounter(&time);
		time.QuadPart *= period::den;
		time.QuadPart /= frequency.QuadPart;
		return time_point(duration(time.QuadPart));
	}
}

#endif