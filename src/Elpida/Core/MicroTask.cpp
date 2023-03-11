//
// Created by klapeto on 27/2/2023.
//

#include "Elpida/Core/MicroTask.hpp"

#include "Elpida/Core/EnvironmentInfo.hpp"

#include <cstdlib>
#include <cmath>

namespace Elpida
{
	Duration MicroTask::Run()
	{
		Duration minimumDuration = GetExecutionMinimumDuration();
		Duration nowOverhead = _environmentInfo->get().GetOverheadsInfo().GetNowOverhead();
		Duration loopOverhead = _environmentInfo->get().GetOverheadsInfo().GetLoopOverhead();

		Size iterations = 1;
		auto currentDuration = Duration::zero();

		while (currentDuration < minimumDuration * 0.98)
		{
			auto start = Timer::now();
			DoRun(iterations);
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
					double iterationsPerDuration = static_cast<double>(iterations) / currentDuration.count();
					iterationsPerDuration *= 1.1 * minimumDuration.count();
					iterations = std::ceil(iterationsPerDuration + 1.0);
				}
			}
		}

		// Consider the previous run as 'warmups'
		// now the real deal
		auto start = Timer::now();
		DoRun(iterations);
		auto end = Timer::now();

		currentDuration = ToDuration(end - start) - nowOverhead - (loopOverhead * iterations);

		return currentDuration / static_cast<double>(iterations) / static_cast<double>(GetOperationsPerformedPerRun());
	}

	void MicroTask::DoRun()
	{

	}
} // Elpida
