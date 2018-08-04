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
 * Timer.hpp
 *
 *  Created on: 21 Μαρ 2018
 *      Author: klapeto
 */

#ifndef ELPIDA_TIMER_HPP_
#define ELPIDA_TIMER_HPP_

#include <cstddef>
#include <chrono>

namespace Elpida
{

	class Timer
	{
		public:

			typedef std::chrono::high_resolution_clock Clock;

			static Clock::time_point now();

			static int64_t getNowOverhead()
			{
				static int64_t returnValue = getNowOverheadImpl();
				return returnValue;
			}

		private:
			static int64_t getNowOverheadImpl()
			{
				int64_t returnValue = 0xFFFFFFFF;
				for (int i = 0; i < 20; ++i)
				{
					auto start = Clock::now();
					auto lag = std::chrono::duration_cast<std::chrono::nanoseconds>(Clock::now() - start).count();
					if (lag < returnValue)
					{
						returnValue = lag;
					}
				}
				return returnValue;
			}
	};

} /* namespace Elpida */

#endif /* ELPIDA_TIMER_HPP_ */
