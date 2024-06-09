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
			const MemoryInfoModel& memoryInfoModel,
			BenchmarkExecutionService& executionService)
			:FullBenchmarkInstance(benchmark, timingModel, topologyModel, memoryInfoModel, executionService)
	{

	}

	std::string SvgRasterizationMultiThread::GetName() const
	{
		return "Svg Rasterization (Multi Thread)";
	}

	FullBenchmarkInstanceResult SvgRasterizationMultiThread::Run()
	{
		_benchmark.GetConfigurations()[0].SetValue("1");
		_benchmark.GetConfigurations()[1].SetValue("0.06");
		_benchmark.GetConfigurations()[2].SetValue("32");

		auto svgRasterizationMulti = _executionService.Execute(
				_benchmark,
				{},
				_timingModel.GetNowOverhead().count(),
				_timingModel.GetLoopOverhead().count(),
				_timingModel.GetVirtualCallOverhead().count(),
				false,
				false,
				ConcurrencyMode::CopyInput);

		auto& taskResults = svgRasterizationMulti.GetTaskResults();

		auto& rasterizationResult = taskResults[0];
		Score multiCoreScore = (rasterizationResult.GetInputSize() / rasterizationResult.GetDuration().count());

		return FullBenchmarkInstanceResult(std::move(svgRasterizationMulti), 0, multiCoreScore, 0);
	}
} // Application
// Elpida