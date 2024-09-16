//
// Created by klapeto on 27/2/2023.
//

#include "Elpida/Core/MicroTask.hpp"

#include "Elpida/Core/EnvironmentInfo.hpp"
#include "Elpida/Core/TimingUtilities.hpp"

#include <thread>


namespace Elpida
{
	Duration MicroTask::Run()
	{
		Duration minimumDuration = GetExecutionMinimumDuration();
		Duration nowOverhead = _environmentInfo->get().GetOverheadsInfo().GetNowOverhead();
		Duration loopOverhead = _environmentInfo->get().GetOverheadsInfo().GetLoopOverhead();

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

	void MicroTask::DoRun()
	{

	}

	void MicroTask::OnBeforeRun(Iterations iterations)
	{

	}

	Duration MicroTask::GetExecutionMinimumDuration()
	{
		return Seconds(3);
	}
} // Elpida
