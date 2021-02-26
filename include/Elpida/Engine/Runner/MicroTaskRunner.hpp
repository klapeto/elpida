/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2021  Ioannis Panagiotopoulos
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
// Created by klapeto on 6/2/21.
//

#ifndef SRC_ELPIDA_ENGINE_RUNNER_MICROTASKRUNNER_HPP
#define SRC_ELPIDA_ENGINE_RUNNER_MICROTASKRUNNER_HPP

#include <algorithm>

#include "Elpida/Utilities/Duration.hpp"
#include "Elpida/Timer.hpp"

namespace Elpida
{
	class MicroTaskRunner
	{
	public:
		template<class TCallable>
		static Duration measure(const Duration& minimumDuration,
			size_t minimumIterations,
			const Duration& nowOverhead,
			const Duration& loopOverhead,
			TCallable callable)
		{
			size_t iterations = std::max(static_cast<size_t>(1), minimumIterations);
			auto currentDuration = Duration::zero();

			while (currentDuration < minimumDuration * 0.98)
			{
				auto start = Timer::now();
				callable(iterations);
				auto end = Timer::now();

				currentDuration = ToDuration(end - start) - nowOverhead - (loopOverhead * iterations);
				if (currentDuration > minimumDuration)
				{
					break;
				}
				else
				{
					if (currentDuration < minimumDuration / 10.0)
					{
						iterations *= 10;
					}
					else
					{
						double iterationsPerDuration = iterations / currentDuration.count();
						iterationsPerDuration *= 1.1 * minimumDuration.count();
						iterations = iterationsPerDuration + 1;
					}
				}
			}

			// Consider the previous run as 'warmups'
			// now the real deal
			auto start = Timer::now();
			callable(iterations);
			auto end = Timer::now();

			currentDuration = ToDuration(end - start) - nowOverhead - (loopOverhead * iterations);

			return currentDuration / static_cast<double>(iterations);
		}


		template<class TCallable>
		static Duration measure(size_t iterations,
			const Duration& nowOverhead,
			const Duration& loopOverhead,
			TCallable callable)
		{
			auto currentDuration = Duration::zero();

			auto start = Timer::now();
			callable(iterations);
			auto end = Timer::now();

			currentDuration = ToDuration(end - start) - nowOverhead - (loopOverhead * iterations);

			return currentDuration / static_cast<double>(iterations);
		}

		MicroTaskRunner() = delete;
		~MicroTaskRunner() = delete;
	};
}

#endif //SRC_ELPIDA_ENGINE_RUNNER_MICROTASKRUNNER_HPP
