//
// Created by klapeto on 16/9/2023.
//

#include "OverheadsCalculator.hpp"
#include "Elpida/Core/Duration.hpp"
#include "Elpida/Core/TimingUtilities.hpp"
#include "Elpida/Core/ThreadTask.hpp"
#include "Elpida/Core/OverheadsInfo.hpp"
#include "DummyClass.hpp"

namespace Elpida::Application
{
	static Duration CalculateLoopOverhead()
	{
		return Seconds(
			1.0 / (double)TimingUtilities::GetIterationsNeededForExecutionTime(Seconds(1),
				Seconds(0),
				Seconds(0),
				[](auto x)
				{
				  while (x--);
				}));
	}

	static Duration CalculateNowOverhead(Duration loopOverhead)
	{
		return Seconds(
			1.0 / (double)TimingUtilities::GetIterationsNeededForExecutionTime(Seconds(1),
				Seconds(0),
				loopOverhead,
				[](auto x)
				{
				  while (x--) std::chrono::high_resolution_clock::now();
				}));
	}

	static Duration CalculateVCallOverhead(Duration loopOverhead, Duration nowOverhead)
	{
		Base* base = new Derived();
		auto duration = Seconds(1.0
			/ (double)TimingUtilities::GetIterationsNeededForExecutionTime(Seconds(1),
				nowOverhead,
				loopOverhead,
				[base](auto x)
				{
				  auto p = base;
				  while (x--) p->Foo();
				}));
		delete base;

		return duration;
	}

	OverheadsInfo OverheadsCalculator::CalculateOverheads(const TopologyInfo& topologyInfo)
	{
		ThreadTask::PinCurrentThreadToProcessor(topologyInfo.GetAllProcessingUnits().back());

		std::vector<std::thread> threads;
		Duration loopOverhead = CalculateLoopOverhead();

		Duration nowOverhead = CalculateNowOverhead(loopOverhead);

		Duration vCallOverhead = CalculateVCallOverhead(loopOverhead, nowOverhead);

		return OverheadsInfo(nowOverhead, loopOverhead, vCallOverhead);
	}
} // Application