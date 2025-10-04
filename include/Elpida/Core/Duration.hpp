/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

//
// Created by klapeto on 27/2/2023.
//

#ifndef _ELPIDA_DURATION_HPP
#define _ELPIDA_DURATION_HPP

#include <chrono>

namespace Elpida
{
	using TimeStamp = std::chrono::time_point<std::chrono::high_resolution_clock>;
	using NanoSeconds = std::chrono::duration<double, std::nano>;
	using MicroSeconds = std::chrono::duration<double, std::micro>;
	using MilliSeconds = std::chrono::duration<double, std::milli>;
	using Seconds = std::chrono::duration<double, std::ratio<1, 1>>;
	using Duration = Seconds;

	using Timer = std::chrono::high_resolution_clock;

	template<typename T>
	constexpr Duration ToDuration(const T& duration)
	{
		return std::chrono::duration_cast<Duration>(duration);
	}

	template<typename TR, typename T>
	constexpr TR DurationCast(const T& duration)
	{
		return std::chrono::duration_cast<T>(duration);
	}

}

#endif //_ELPIDA_DURATION_HPP
