/*
 * Timer.hpp
 *
 *  Created on: 21 Μαρ 2018
 *      Author: klapeto
 */

#ifndef SRC_TIMER_HPP_
#define SRC_TIMER_HPP_

#include <cstddef>
#include <chrono>

namespace Elpida
{

	class Timer
	{
		public:

			typedef std::chrono::high_resolution_clock Clock;

			inline static Clock::time_point now()
			{
				return Clock::now();
			}

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

#endif /* SRC_TIMER_HPP_ */
