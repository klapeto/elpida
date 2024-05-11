//
// Created by klapeto on 11/5/2024.
//

#include "SvgRasterizationMultiThread.hpp"

#include "Models/Benchmark/BenchmarkModel.hpp"
#include "Models/SystemInfo/TimingModel.hpp"
#include "Models/SystemInfo/TopologyModel.hpp"
#include "Core/BenchmarkExecutionService.hpp"

#include <thread>

namespace Elpida::Application
{
	SvgRasterizationMultiThread::SvgRasterizationMultiThread(const BenchmarkModel& benchmark,
			const TimingModel& timingModel, const TopologyModel& topologyModel,
			BenchmarkExecutionService& executionService)
			:FullBenchmarkInstance(benchmark, timingModel, topologyModel, executionService)
	{

	}

	std::string SvgRasterizationMultiThread::GetName() const
	{
		return "Svg Rasterization (Multi Thread)";
	}

	FullBenchmarkInstanceResult SvgRasterizationMultiThread::Run()
	{
		auto targetSamples = std::max((std::size_t)1,
				(std::size_t)(8192 * ((double)_timingModel.GetIterationsPerSecond() / std::giga::num)));
		_benchmark.GetConfigurations()[0].SetValue(std::to_string(std::thread::hardware_concurrency()));
		_benchmark.GetConfigurations()[1].SetValue("0.06");
		_benchmark.GetConfigurations()[2].SetValue(std::to_string(targetSamples));
		_benchmark.GetConfigurations()[3].SetValue("true");
		_benchmark.GetConfigurations()[4].SetValue("true");
		_benchmark.GetConfigurations()[5].SetValue("true");

		auto svgRasterizationMulti = _executionService.Execute(
				_benchmark,
				{},
				_timingModel.GetNowOverhead().count(),
				_timingModel.GetLoopOverhead().count(),
				_timingModel.GetVirtualCallOverhead().count(),
				32.0,
				32.0,
				false,
				false);

		auto& taskResults = svgRasterizationMulti.GetTaskResults();

		auto& rasterizationResult = taskResults[0];
		Score multiCoreScore =
				(rasterizationResult.GetInputSize() / rasterizationResult.GetDuration().count()) / std::mega::num;

		return FullBenchmarkInstanceResult(std::move(svgRasterizationMulti), 0, multiCoreScore, 0);
	}
} // Application
// Elpida