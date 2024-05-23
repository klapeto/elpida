//
// Created by klapeto on 11/5/2024.
//

#include "MemoryReadBandwidth.hpp"

#include "Models/Benchmark/BenchmarkModel.hpp"
#include "Models/SystemInfo/TimingModel.hpp"
#include "Models/SystemInfo/TopologyModel.hpp"
#include "Core/BenchmarkExecutionService.hpp"

namespace Elpida::Application
{
	MemoryReadBandwidth::MemoryReadBandwidth(const BenchmarkModel& benchmark, const TimingModel& timingModel,
			const TopologyModel& topologyModel, BenchmarkExecutionService& executionService)
			:FullBenchmarkInstance(benchmark, timingModel, topologyModel, executionService)
	{

	}

	std::string MemoryReadBandwidth::GetName() const
	{
		return "Memory read bandwidth";
	}

	FullBenchmarkInstanceResult MemoryReadBandwidth::Run()
	{
		auto bandwidthResult =  _executionService.Execute(
				_benchmark,
				{},
				_timingModel.GetNowOverhead().count(),
				_timingModel.GetLoopOverhead().count(),
				_timingModel.GetVirtualCallOverhead().count(),
				false,
				true,
				ConcurrencyMode::ShareInput);


		Score memoryScore = bandwidthResult.GetScore() / std::giga::num;

		return FullBenchmarkInstanceResult(std::move(bandwidthResult), 0, 0, memoryScore);
	}
} // Application
// Elpida