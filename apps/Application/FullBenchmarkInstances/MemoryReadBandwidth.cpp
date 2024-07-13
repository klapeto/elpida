//
// Created by klapeto on 11/5/2024.
//

#include "MemoryReadBandwidth.hpp"

#include "Models/Benchmark/BenchmarkModel.hpp"
#include "Models/SystemInfo/TimingModel.hpp"
#include "Models/SystemInfo/TopologyModel.hpp"
#include "Models/SystemInfo/MemoryInfoModel.hpp"
#include "Core/BenchmarkExecutionService.hpp"

#include <algorithm>

namespace Elpida::Application
{
	MemoryReadBandwidth::MemoryReadBandwidth(const BenchmarkModel& benchmark, const TimingModel& timingModel,
			const TopologyModel& topologyModel, const MemoryInfoModel& memoryInfoModel,
			BenchmarkExecutionService& executionService)
			:FullBenchmarkInstance(benchmark, timingModel, topologyModel, memoryInfoModel, executionService)
	{

	}

	std::string MemoryReadBandwidth::GetName() const
	{
		return "Memory read bandwidth";
	}

	FullBenchmarkInstanceResult MemoryReadBandwidth::Run()
	{
		std::size_t cacheSize = 16 * 1024 * 1024;
		for (auto& processor : _topologyModel.GetSelectedLeafNodes())
		{
			auto cache = processor.get().GetLastCache();
			if (cache.has_value())
			{
				cacheSize = std::max(cacheSize, cache->get().GetSize().value());
			}
		}

		auto maxTotalSize = _memoryInfoModel.GetTotalSize() / 2;
		auto currentTotalSize = _topologyModel.GetSelectedLeafNodes().size() * cacheSize * 8;

		if (currentTotalSize > maxTotalSize)
		{
			cacheSize = maxTotalSize / _topologyModel.GetSelectedLeafNodes().size() / 8;
		}

		_benchmark.GetConfigurations()[0].SetValue(std::to_string(cacheSize * 8));


		auto bandwidthResult = _executionService.Execute(
				_benchmark,
				{},
				_timingModel.GetNowOverhead().count(),
				_timingModel.GetLoopOverhead().count(),
				_timingModel.GetVirtualCallOverhead().count(),
				false,
				true,
				ConcurrencyMode::ShareInput);

		Score memoryScore = bandwidthResult.GetResult() / std::giga::num;

		return FullBenchmarkInstanceResult(std::move(bandwidthResult), 0, 0, memoryScore);
	}

	std::string MemoryReadBandwidth::GetUuid() const
	{
		return "7cfa7e2e-70a5-4926-b1b2-14ce6304f930";
	}
} // Application
// Elpida