//
// Created by klapeto on 16/9/2023.
//

#include "Elpida/Core/TimingCalculator.hpp"

#include "Elpida/Core/Duration.hpp"
#include "Elpida/Core/TimingUtilities.hpp"
#include "Elpida/Core/TimingInfo.hpp"

namespace Elpida
{
	const constexpr Duration DefaultTestDuration = Seconds(1);

	Duration TimingCalculator::CalculateLoopOverheadFast()
	{
		return Seconds(
				1.0 / (double)TimingUtilities::GetIterationsNeededForExecutionTime(MilliSeconds(100),
						Seconds(0),
						Seconds(0),
						[](volatile auto x)
						{
							while (x-- > 0);
						}));
	}

	static Duration CalculateLoopOverhead()
	{
		return Seconds(
			1.0 / (double)TimingUtilities::GetIterationsNeededForExecutionTime(DefaultTestDuration,
				Seconds(0),
				Seconds(0),
				[](volatile auto x)
				{
				  while (x-- > 0);
				}));
	}

	static Duration CalculateNowOverhead(Duration loopOverhead)
	{
		return Seconds(
			1.0 / (double)TimingUtilities::GetIterationsNeededForExecutionTime(DefaultTestDuration,
				Seconds(0),
				loopOverhead,
				[](volatile auto x)
				{
				  while (x-- > 0) std::chrono::high_resolution_clock::now();
				}));
	}

	TimingInfo TimingCalculator::CalculateTiming()
	{
		Duration loopOverhead;
		Duration nowOverhead;

		loopOverhead = CalculateLoopOverhead();
		nowOverhead = CalculateNowOverhead(loopOverhead);

		return TimingInfo(nowOverhead,
				loopOverhead,
				1.0 / loopOverhead.count());
	}
} // Application