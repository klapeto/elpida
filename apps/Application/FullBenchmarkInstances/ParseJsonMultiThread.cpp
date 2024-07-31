//
// Created by klapeto on 30/7/2024.
//

#include "ParseJsonMultiThread.hpp"
#include "Models/BenchmarkRunConfigurationModel.hpp"

#include "Models/Benchmark/BenchmarkModel.hpp"
#include "Models/SystemInfo/TimingModel.hpp"
#include "Models/SystemInfo/TopologyModel.hpp"
#include "Core/BenchmarkExecutionService.hpp"

namespace Elpida::Application
	{
		std::string ParseJsonMultiThread::GetName() const
		{
			return "Svg parsing (Multi Thread)";
		}

		std::string ParseJsonMultiThread::GetUuid() const
		{
			return "35f22cbc-16e7-4c35-aebf-029dac328e46";
		}

		FullBenchmarkInstanceResult ParseJsonMultiThread::Run()
		{
			_benchmark.GetConfigurations()[0].SetValue("4096");

			auto svgRasterizationSingle = _executionService.Execute(
					_benchmark,
					{},
					_timingModel.GetNowOverhead().count(),
					_timingModel.GetLoopOverhead().count(),
					false,
					false,
					ConcurrencyMode::CopyInput);

			auto rasterizationResult = svgRasterizationSingle.GetResult() / std::mega::num;
			Score multiThreadScore = rasterizationResult;

			return FullBenchmarkInstanceResult(std::move(svgRasterizationSingle), 0, multiThreadScore);
		}

		ParseJsonMultiThread::ParseJsonMultiThread(const BenchmarkModel& benchmark, const TimingModel& timingModel,
				const TopologyModel& topologyModel, const MemoryInfoModel& memoryInfoModel,
				BenchmarkExecutionService& executionService)
				:FullBenchmarkInstance(benchmark, timingModel, topologyModel, memoryInfoModel, executionService)
		{

		}
	} // Application
// Elpida