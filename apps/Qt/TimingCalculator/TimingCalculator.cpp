//
// Created by klapeto on 16/9/2023.
//

#include "TimingCalculator.hpp"

#include "Elpida/Core/Duration.hpp"
#include "Elpida/Core/TimingUtilities.hpp"
#include "Elpida/Core/ThreadTask.hpp"
#include "Elpida/Core/TimingInfo.hpp"
#include "Elpida/Core/Pool.hpp"
#include "Elpida/Core/Topology/ProcessingUnitNode.hpp"
#include "Elpida/Core/Repeat.hpp"

#include "DummyClass.hpp"
#include <thread>
#include <vector>

namespace Elpida::Application
{
	const constexpr Duration DefaultTestDuration = Seconds(1);

	const constexpr Duration TestDurations[] = {
		MilliSeconds(100),
		MilliSeconds(250),
		MilliSeconds(500),
	};

	const constexpr float TimingTestVariations[]{
		1.015,
		1.02,
		1.035
	};

	constexpr double DiffVarianceMaxMargin = 0.0025;

	struct TimingTest
	{
		Duration testDuration;
		bool success;
		std::thread thread;
	};

	static Duration CalculateLoopOverhead()
	{
		return Seconds(
			1.0 / (double)TimingUtilities::GetIterationsNeededForExecutionTime(DefaultTestDuration,
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
			1.0 / (double)TimingUtilities::GetIterationsNeededForExecutionTime(DefaultTestDuration,
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
			/ (double)TimingUtilities::GetIterationsNeededForExecutionTime(DefaultTestDuration,
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

	static Size GetIterationsForTime(const Duration& testDuration, const Duration& nowOverhead)
	{
		long* dummy = reinterpret_cast<long*>(&dummy);    // Points to itself
		long** ptr = static_cast<long**>(&dummy);    // Points to dummy
		return TimingUtilities::GetIterationsNeededForExecutionTime(testDuration,
			nowOverhead,
			Seconds(0),
			[&](auto x)
			{
			  while (x--)
			  {
				  REPEAT_1000(ptr = reinterpret_cast<long**>(*ptr));
			  }
			});
	}

	static void TestTiming(TimingTest& test, const Duration& nowOverhead)
	{
		auto baseIterations = static_cast<double>(GetIterationsForTime(test.testDuration, nowOverhead));

		for (auto point : TimingTestVariations)
		{
			auto thisIterations = static_cast<double>(GetIterationsForTime(test.testDuration * point, nowOverhead));

			auto diff = std::abs(thisIterations - (baseIterations * point));
			if (diff / baseIterations > DiffVarianceMaxMargin)
			{
				test.success = false;
				return;
			}
		}

		test.success = true;
	}

	TimingInfo TimingCalculator::CalculateTiming(const TopologyInfo& topologyInfo)
	{
		auto& processors = topologyInfo.GetAllProcessingUnits();
		ThreadTask::PinCurrentThreadToProcessor(processors.back());

		Duration loopOverhead = CalculateLoopOverhead();
		Duration nowOverhead = CalculateNowOverhead(loopOverhead);

		Pool<Ref<const ProcessingUnitNode>> processorPool;
		processorPool.Add(processors);

		Duration vCallOverhead;
		std::thread vCallThread = std::thread([&]()
		{
		  auto processor = processorPool.Rent();
		  ThreadTask::PinCurrentThreadToProcessor(processor.Get());
		  vCallOverhead = CalculateVCallOverhead(loopOverhead, nowOverhead);
		});

		std::vector<TimingTest> timingTests;
		timingTests.reserve(sizeof(TestDurations) / sizeof(Duration));
		for (auto& duration : TestDurations)
		{
			timingTests.push_back({
				.testDuration = duration,
				.success = false,
				.thread = {}
			});
		}

		for (auto& test : timingTests)
		{
			test.thread = std::thread([&]()
			{
			  auto processor = processorPool.Rent();
			  ThreadTask::PinCurrentThreadToProcessor(processor.Get());
			  TestTiming(test, nowOverhead);
			});
		}

		Duration minimumTimeForStableMeasurement = Seconds(1);
		for (auto& test : timingTests)
		{
			test.thread.join();
			if (test.success && test.testDuration < minimumTimeForStableMeasurement)
			{
				minimumTimeForStableMeasurement = test.testDuration;
			}
		}
		vCallThread.join();

		return TimingInfo(nowOverhead, loopOverhead, vCallOverhead, minimumTimeForStableMeasurement, 1.0 / loopOverhead.count());
	}
} // Application