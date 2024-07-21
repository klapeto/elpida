//
// Created by klapeto on 11/5/2024.
//

#include "SvgRasterizationSingleThread.hpp"

#include "Models/Benchmark/BenchmarkModel.hpp"
#include "Models/SystemInfo/TimingModel.hpp"
#include "Models/SystemInfo/TopologyModel.hpp"
#include "Core/BenchmarkExecutionService.hpp"

namespace Elpida::Application
{
	SvgRasterizationSingleThread::SvgRasterizationSingleThread(const BenchmarkModel& benchmark,
			const TimingModel& timingModel,
			const TopologyModel& topologyModel,
			const MemoryInfoModel& memoryInfoModel,
			BenchmarkExecutionService& executionService)
			:FullBenchmarkInstance(benchmark, timingModel, topologyModel, memoryInfoModel, executionService)
	{
	}

	std::string SvgRasterizationSingleThread::GetName() const
	{
		return "Svg Rasterization (Single Thread)";
	}

	FullBenchmarkInstanceResult SvgRasterizationSingleThread::Run()
	{
		_benchmark.GetConfigurations()[0].SetValue("1");
		_benchmark.GetConfigurations()[1].SetValue("0.06");
		_benchmark.GetConfigurations()[2].SetValue("32");

		auto svgRasterizationSingle = _executionService.Execute(
				_benchmark,
				{ _topologyModel.GetFastestProcessor() },
				_timingModel.GetNowOverhead().count(),
				_timingModel.GetLoopOverhead().count(),
				false,
				false,
				ConcurrencyMode::None);

		auto rasterizationResult = svgRasterizationSingle.GetResult();
		Score singleCoreScore = rasterizationResult;

		return FullBenchmarkInstanceResult(std::move(svgRasterizationSingle), singleCoreScore, 0);
	}

	std::string SvgRasterizationSingleThread::GetUuid() const
	{
		return "e7ab603e-6c05-428b-9851-1e0a3729e022";
	}
} // Application
// Elpida