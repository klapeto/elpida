//
// Created by klapeto on 11/5/2024.
//

#include "MemoryLatency.hpp"

#include "Models/Benchmark/BenchmarkModel.hpp"
#include "Models/SystemInfo/TimingModel.hpp"
#include "Models/SystemInfo/TopologyModel.hpp"
#include "Core/BenchmarkExecutionService.hpp"

namespace Elpida::Application
{
	MemoryLatency::MemoryLatency(const BenchmarkModel& benchmark, const TimingModel& timingModel,
			const TopologyModel& topologyModel, BenchmarkExecutionService& executionService)
			:FullBenchmarkInstance(benchmark, timingModel, topologyModel, executionService)
	{

	}

	std::string MemoryLatency::GetName() const
	{
		return "Memory latency";
	}

	FullBenchmarkInstanceResult MemoryLatency::Run()
	{
		auto latencyResult = _executionService.Execute(
				_benchmark,
				{0},
				_timingModel.GetNowOverhead().count(),
				_timingModel.GetLoopOverhead().count(),
				_timingModel.GetVirtualCallOverhead().count(),
				false,
				true,
				ConcurrencyMode::ShareInput);

		Score memoryScore = (1.0 / std::micro::den) / latencyResult.GetScore();

		return FullBenchmarkInstanceResult(std::move(latencyResult), 0, 0, memoryScore);
	}
} // Application
// Elpida