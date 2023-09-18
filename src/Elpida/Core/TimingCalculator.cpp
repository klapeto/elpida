//
// Created by klapeto on 16/9/2023.
//

#include "Elpida/Core/TimingCalculator.hpp"

#include "Elpida/Core/Duration.hpp"
#include "Elpida/Core/TimingUtilities.hpp"
#include "Elpida/Core/ThreadTask.hpp"
#include "Elpida/Core/TimingInfo.hpp"
#include "Elpida/Core/Pool.hpp"
#include "Elpida/Core/Repeat.hpp"

#include "Elpida/Core/DummyClass.hpp"
#include "Elpida/Core/Topology/TopologyNode.hpp"
#include <thread>
#include <tuple>
#include <vector>

namespace Elpida
{
	const constexpr Duration DefaultTestDuration = Seconds(1);

	const constexpr std::tuple<Duration, TimingStability> TestDurations[] = {
		std::make_tuple(MilliSeconds(5), TimingStability::ExtremelyStable),
		std::make_tuple(MilliSeconds(10), TimingStability::VeryStable),
		std::make_tuple(MilliSeconds(50), TimingStability::Stable),
		std::make_tuple(MilliSeconds(100), TimingStability::Unstable),
		std::make_tuple(MilliSeconds(250), TimingStability::VeryUnstable),
		std::make_tuple(MilliSeconds(500), TimingStability::ExtremelyUnstable)
	};

	const constexpr float TimingTestVariations[]{
		1.015,
		1.02,
		1.035
	};

	constexpr double DiffVarianceMaxMargin = 0.0025;

	struct TimingTest
	{
		std::tuple<Duration, TimingStability> testDuration;
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

	static Size GetIterationsForTime(const Duration& testDuration, const Duration& nowOverhead)
	{
		long* dummy = reinterpret_cast<long*>(&dummy);    // Points to itself
		long** ptr = static_cast<long**>(&dummy);    // Points to dummy
		return TimingUtilities::GetIterationsNeededForExecutionTime(testDuration,
			nowOverhead,
			Seconds(0),
			[&](auto x)
			{
			  while (x-- > 0)
			  {
				  REPEAT_10(ptr = reinterpret_cast<long**>(*ptr));
			  }
			});
	}

	static void TestTiming(TimingTest& test, const Duration& nowOverhead)
	{
		auto duration = get<Duration>(test.testDuration);
		auto baseIterations = static_cast<double>(GetIterationsForTime(duration, nowOverhead));

		for (auto point : TimingTestVariations)
		{
			auto thisIterations = static_cast<double>(GetIterationsForTime(duration * point, nowOverhead));

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
		Duration loopOverhead;
		Duration nowOverhead;
		Duration vCallOverhead;
		Duration minimumTimeForStableMeasurement;
		TimingStability stability = TimingStability::ExtremelyUnstable;

		std::thread([&]()
		{
		  auto& processors = topologyInfo.GetAllCores();
		  ThreadTask::PinCurrentThreadToProcessor(0);

		  loopOverhead = CalculateLoopOverhead();
		  nowOverhead = CalculateNowOverhead(loopOverhead);

		  Pool<Ref<const TopologyNode>> processorPool;
		  processorPool.Add(processors);

		  std::thread vCallThread = std::thread([&]()
		  {
			auto processor = processorPool.Rent();
			ThreadTask::PinCurrentThreadToProcessor(processor.Get().get().GetChildren().front()->GetOsIndex().value());
			vCallOverhead = CalculateVCallOverhead(loopOverhead, nowOverhead);
		  });

		  std::vector<TimingTest> timingTests;
		  timingTests.reserve(sizeof(TestDurations) / sizeof(std::tuple<Duration, TimingStability>));
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
				ThreadTask::PinCurrentThreadToProcessor(processor.Get().get().GetChildren().front()->GetOsIndex()
					.value());
				TestTiming(test, nowOverhead);
			  });
		  }

		  minimumTimeForStableMeasurement = Seconds(1);
		  for (auto& test : timingTests)
		  {
			  test.thread.join();
			  auto duration = get<Duration>(test.testDuration);
			  if (test.success && duration < minimumTimeForStableMeasurement)
			  {
				  minimumTimeForStableMeasurement = duration;
				  stability = get<TimingStability>(test.testDuration);
			  }
		  }
		  vCallThread.join();

		}).join();

		return TimingInfo(nowOverhead,
			loopOverhead,
			vCallOverhead,
			minimumTimeForStableMeasurement,
			1.0 / loopOverhead.count(),
			stability);
	}
} // Application