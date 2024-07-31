//
// Created by klapeto on 30/7/2024.
//

#include "ParseJsonSingleThread.hpp"

#include "Models/Benchmark/BenchmarkModel.hpp"
#include "Models/SystemInfo/TimingModel.hpp"
#include "Models/SystemInfo/TopologyModel.hpp"
#include "Core/BenchmarkExecutionService.hpp"

namespace Elpida::Application
{
	ParseJsonSingleThread::ParseJsonSingleThread(const BenchmarkModel& benchmark, const TimingModel& timingModel,
			const TopologyModel& topologyModel, const MemoryInfoModel& memoryInfoModel,
			BenchmarkExecutionService& executionService)
			:FullBenchmarkInstance(benchmark, timingModel, topologyModel, memoryInfoModel, executionService)
	{

	}

	FullBenchmarkInstanceResult ParseJsonSingleThread::Run()
	{
		_benchmark.GetConfigurations()[0].SetValue("4096");

		auto svgRasterizationSingle = _executionService.Execute(
				_benchmark,
				{ _topologyModel.GetFastestProcessor() },
				_timingModel.GetNowOverhead().count(),
				_timingModel.GetLoopOverhead().count(),
				false,
				false,
				ConcurrencyMode::None);

		auto rasterizationResult = svgRasterizationSingle.GetResult() / std::mega::num;
		Score singleCoreScore = rasterizationResult;

		return FullBenchmarkInstanceResult(std::move(svgRasterizationSingle), singleCoreScore, 0);
	}

	std::string ParseJsonSingleThread::GetUuid() const
	{
		return "cc53b078-75aa-41c0-8885-b381bc1a4bda";
	}

	std::string ParseJsonSingleThread::GetName() const
	{
		return "Json Parsing";
	}
} // Application
// Elpida