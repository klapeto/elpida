/*
 *  Copyright (c) 2024-2025  Ioannis Panagiotopoulos
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
// Created by klapeto on 22/9/2024.
//

#include "Elpida/Core/IterationMicroTask.hpp"

#include "Elpida/Core/EnvironmentInfo.hpp"
#include "Elpida/Core/TimingUtilities.hpp"
#include "Elpida/Core/BenchmarkRunContext.hpp"

#include <thread>

namespace Elpida
{
	Duration IterationMicroTask::Run()
	{
		auto& context = _benchmarkRunContext.value().get();
		auto& overheads = context.GetEnvironmentInfo().GetOverheadsInfo();

		Duration minimumDuration = std::max(GetExecutionMinimumDuration(), context.GetMinimumMicroTaskDuration());
		Duration nowOverhead = overheads.GetNowOverhead();
		Duration loopOverhead = overheads.GetLoopOverhead();

		auto minimum = ShouldBeMeasured() ? TimingUtilities::GetMinimumIterationsAndDurationNeededForExecutionTime(
				minimumDuration,
				nowOverhead,
				loopOverhead,
				[this](Size iterations)
				{ OnBeforeRun(iterations); },
				[this](auto duration)
				{ return PostProcessDuration(duration); },
				[this](Size iterations)
				{ DoRun(iterations); }) : std::tuple<Iterations, Duration>{ 1, 0 };

		Duration currentDuration;
		auto iterations = std::get<0>(minimum);
		if (iterations != 1)
		{
			// Consider the previous run as 'warmups'
			// now the real deal
			OnBeforeRun(iterations);
			auto start = Timer::now();
			DoRun(iterations);
			auto end = Timer::now();

			currentDuration = ToDuration(end - start) - nowOverhead - (loopOverhead * iterations);
		}
		else
		{
			auto duration = std::get<1>(minimum);
			if (duration.count() == 0)
			{
				// It was never run, so run it at least once
				OnBeforeRun(iterations);
				auto start = Timer::now();
				DoRun(iterations);
				auto end = Timer::now();
				duration = ToDuration(end - start);
			}
			currentDuration = duration - nowOverhead - (loopOverhead * iterations);
		}


		currentDuration = PostProcessDuration(currentDuration);
		currentDuration =
				(currentDuration / static_cast<double>(iterations)) /
				static_cast<double>(GetOperationsPerformedPerRun());

		return currentDuration;
	}

	void IterationMicroTask::DoRun()
	{

	}

	void IterationMicroTask::OnBeforeRun(Iterations iterations)
	{

	}

	Duration IterationMicroTask::GetExecutionMinimumDuration()
	{
		return Seconds(0.1);
	}
} // Elpida