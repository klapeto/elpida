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
			BenchmarkExecutionService& executionService)
			:FullBenchmarkInstance(benchmark, timingModel, topologyModel, executionService)
	{
	}

	std::string SvgRasterizationSingleThread::GetName() const
	{
		return "Svg Rasterization (Single Thread)";
	}

	FullBenchmarkInstanceResult SvgRasterizationSingleThread::Run()
	{
		auto targetSamples = std::max((std::size_t)1,
				(std::size_t)(8192 * ((double)_timingModel.GetIterationsPerSecond() / std::giga::num)));
		_benchmark.GetConfigurations()[0].SetValue("1");
		_benchmark.GetConfigurations()[1].SetValue("0.06");
		_benchmark.GetConfigurations()[2].SetValue(std::to_string(targetSamples));
		_benchmark.GetConfigurations()[3].SetValue("false");
		_benchmark.GetConfigurations()[4].SetValue("false");
		_benchmark.GetConfigurations()[5].SetValue("false");

		auto svgRasterizationSingle = _executionService.Execute(
				_benchmark,
				{ _topologyModel.GetFastestProcessor() },
				_timingModel.GetNowOverhead().count(),
				_timingModel.GetLoopOverhead().count(),
				_timingModel.GetVirtualCallOverhead().count(),
				1.0,
				1.0,
				false,
				false);

		auto& taskResults = svgRasterizationSingle.GetTaskResults();

		auto& rasterizationResult = taskResults[0];
		Score singleCoreScore =
				(rasterizationResult.GetInputSize() / rasterizationResult.GetDuration().count()) / 1000000.0;

		return FullBenchmarkInstanceResult(std::move(svgRasterizationSingle), singleCoreScore, 0, 0);
	}
} // Application
// Elpida