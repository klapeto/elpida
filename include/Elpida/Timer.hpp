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
 * Timer.hpp
 *
 *  Created on: 21 Μαρ 2018
 *      Author: klapeto
 */

#ifndef ELPIDA_TIMER_HPP_
#define ELPIDA_TIMER_HPP_

#include <chrono>

namespace Elpida
{

	class Timer final
	{
	private:
		using Clock = std::chrono::high_resolution_clock;
	public:

		using time_point = Clock::time_point;
		using duration = Clock::duration;
		using rep = Clock::rep;
		using period = Clock::period;

		static time_point now();

		Timer() = delete;
	};

} /* namespace Elpida */

#endif /* ELPIDA_TIMER_HPP_ */
