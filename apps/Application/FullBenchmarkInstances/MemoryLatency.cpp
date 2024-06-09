//
// Created by klapeto on 11/5/2024.
//

#include "MemoryLatency.hpp"

#include "Models/Benchmark/BenchmarkModel.hpp"
#include "Models/SystemInfo/TimingModel.hpp"
#include "Models/SystemInfo/TopologyModel.hpp"
#include "Core/BenchmarkExecutionService.hpp"

#include <algorithm>

namespace Elpida::Application
{
	MemoryLatency::MemoryLatency(const BenchmarkModel& benchmark, const TimingModel& timingModel,
			const TopologyModel& topologyModel,
			const MemoryInfoModel& memoryInfoModel,
			BenchmarkExecutionService& executionService)
			:FullBenchmarkInstance(benchmark, timingModel, topologyModel, memoryInfoModel, executionService)
	{

	}

	std::string MemoryLatency::GetName() const
	{
		return "Memory latency";
	}

	FullBenchmarkInstanceResult MemoryLatency::Run()
	{
		std::size_t cacheSize = 16 * 1024 * 1024;

		auto cache = _topologyModel.GetLeafNodes().at(0).get().GetLastCache();
		if (cache.has_value())
		{
			cacheSize = std::max(cacheSize, cache->get().GetSize().value());
		}

		_benchmark.GetConfigurations()[0].SetValue(std::to_string(cacheSize * 8));

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