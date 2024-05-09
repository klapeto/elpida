//
// Created by klapeto on 16/9/2023.
//

#include "Elpida/Core/TimingCalculator.hpp"

#include "Elpida/Core/Duration.hpp"
#include "Elpida/Core/TimingUtilities.hpp"
#include "Elpida/Core/TimingInfo.hpp"

#include "Elpida/Core/DummyClass.hpp"
#include <thread>

namespace Elpida
{
	const constexpr Duration DefaultTestDuration = Seconds(1);


	Duration TimingCalculator::CalculateLoopOverheadFast()
	{
		return Seconds(
				1.0 / (double)TimingUtilities::GetIterationsNeededForExecutionTime(MilliSeconds(100),
						Seconds(0),
						Seconds(0),
						[](auto x)
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
				[](auto x)
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
				[](auto x)
				{
				  while (x-- > 0) std::chrono::high_resolution_clock::now();
				}));
	}

	static Duration CalculateVCallOverhead(Duration loopOverhead, Duration nowOverhead)
	{
		Base* base = new Derived();
		auto duration = Seconds(1.0
			/ (double)TimingUtilities::GetIterationsNeededForExecutionTime(DefaultTestDuration,
				nowOverhead,
				loopOverhead,
				[base](auto x)
				{
				  auto p = base;
				  while (x-- > 0) p->Foo();
				}));
		delete base;

		return duration;
	}

	TimingInfo TimingCalculator::CalculateTiming()
	{
		Duration loopOverhead;
		Duration nowOverhead;
		Duration vCallOverhead;


		loopOverhead = CalculateLoopOverhead();
		nowOverhead = CalculateNowOverhead(loopOverhead);
		vCallOverhead = CalculateVCallOverhead(loopOverhead, nowOverhead);

		return TimingInfo(nowOverhead,
				loopOverhead,
				vCallOverhead,
				1.0 / loopOverhead.count());
	}
} // Application