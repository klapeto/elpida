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

		Size iterations = ShouldBeMeasured() ? TimingUtilities::GetMinimumIterationsNeededForExecutionTime(
			minimumDuration,
			nowOverhead,
			loopOverhead,
			[this](){OnBeforeRun();},
			[this](auto duration){return PostProcessDuration(duration);},
			[this](Size iterations){DoRun(iterations);}) : 1;

		// Consider the previous run as 'warmups'
		// now the real deal
		OnBeforeRun();
		auto start = Timer::now();
		DoRun(iterations);
		auto end = Timer::now();

		auto currentDuration = ToDuration(end - start) - nowOverhead - (loopOverhead * iterations);

		currentDuration = PostProcessDuration(currentDuration);
		return currentDuration / static_cast<double>(iterations) / static_cast<double>(GetOperationsPerformedPerRun());
	}

	void MicroTask::DoRun()
	{

	}

	void MicroTask::OnBeforeRun()
	{

	}
} // Elpida
