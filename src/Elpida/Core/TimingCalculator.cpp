//
// Created by klapeto on 16/9/2023.
//

#include "Elpida/Core/TimingCalculator.hpp"

#include "Elpida/Core/Duration.hpp"
#include "Elpida/Core/TimingUtilities.hpp"
#include "Elpida/Core/TimingInfo.hpp"
#include "Elpida/Core/Topology/TopologyInfo.hpp"

#include "Elpida/Core/DummyClass.hpp"
#include <thread>

namespace Elpida
{
	const constexpr Duration DefaultTestDuration = Seconds(1);


	static Duration CalculateLoopOverheadFast()
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

	TimingInfo TimingCalculator::CalculateTiming(const TopologyInfo& topologyInfo)
	{
		Duration loopOverhead = Seconds(6546513);
		Duration nowOverhead;
		Duration vCallOverhead;

		unsigned int highestCore = 0;

		std::thread([&]
		{
			ProcessingUnitNode::PinThreadToProcessor(0);
			auto& cores = topologyInfo.GetAllCores();

			for (auto& core : cores)
			{
				auto& pu = core.get().GetChildren().front();

				Duration overhead;

				ProcessingUnitNode::PinThreadToProcessor(pu.get()->GetOsIndex().value());
				//std::this_thread::sleep_for(MilliSeconds(1));
				overhead = CalculateLoopOverheadFast();

				if (overhead < loopOverhead)
				{
					highestCore = pu->GetOsIndex().value();
					loopOverhead = overhead;
				}
			}

			ProcessingUnitNode::PinThreadToProcessor(highestCore);

			loopOverhead = CalculateLoopOverhead();
			nowOverhead = CalculateNowOverhead(loopOverhead);
			vCallOverhead = CalculateVCallOverhead(loopOverhead, nowOverhead);

		}).join();

		return TimingInfo(nowOverhead,
				loopOverhead,
				vCallOverhead,
				1.0 / loopOverhead.count(),
				highestCore);
	}
} // Application