//
// Created by klapeto on 22/10/2023.
//

#include "FullBenchmarkController.hpp"

#include "Models/SystemInfo/TopologyModel.hpp"
#include "Models/SystemInfo/TimingModel.hpp"
#include "Models/BenchmarkRunConfigurationModel.hpp"

#include "Core/BenchmarkExecutionService.hpp"

namespace Elpida::Application
{
	const double Divider = 1000000.0;

	FullBenchmarkController::FullBenchmarkController(FullBenchmarkModel& model,
			const TopologyModel& topologyModel,
			const TimingModel& overheadsModel,
			const BenchmarkRunConfigurationModel& benchmarkRunConfigurationModel,
			BenchmarkExecutionService& benchmarkExecutionService,
			const std::vector<BenchmarkGroupModel>& benchmarkGroups) :
			Controller(model),
			_topologyModel(topologyModel),
			_overheadsModel(overheadsModel),
			_benchmarkRunConfigurationModel(benchmarkRunConfigurationModel),
			_benchmarkExecutionService(benchmarkExecutionService),
			_memoryLatency(nullptr),
			_memoryReadBandwidth(nullptr),
			_svgRasterizationSingle(nullptr),
			_svgRasterizationMulti(nullptr),
			_cancelling(false)
	{
		for (auto& group: benchmarkGroups)
		{
			for (auto& benchmark: group.GetBenchmarks())
			{
				auto name = benchmark.GetName();
				if (name == "Memory latency")
				{
					_memoryLatency = &benchmark;
				}
				else if (name == "Memory read bandwidth")
				{
					_memoryReadBandwidth = &benchmark;
				}
				else if (name == "Svg Rasterization (Multi Thread)")
				{
					_svgRasterizationMulti = &benchmark;
				}
				else if (name == "Svg Rasterization (Single Thread)")
				{
					_svgRasterizationSingle = &benchmark;
				}
			}
		}
		if (!_memoryReadBandwidth || !_memoryLatency)
		{
			throw ElpidaException("Missing benchmarks: Memory benchmarks");
		}

		if (!_svgRasterizationMulti)
		{
			throw ElpidaException("Missing benchmarks: Svg Rasterization (Multi Thread)");
		}

		if (!_svgRasterizationSingle)
		{
			throw ElpidaException("Missing benchmarks: Svg Rasterization (Single Thread)");
		}

		_model.SetTotalBenchmarks(4);
	}

	Promise<> FullBenchmarkController::RunAsync()
	{
		_cancelling = false;
		_model.SetRunning(true);
		std::vector<std::size_t> affinity;
		FullBenchmarkResultModel::Score singleCoreScore = 0;
		FullBenchmarkResultModel::Score multiCoreScore = 0.0;
		FullBenchmarkResultModel::Score memoryScore = 0.0;
		std::vector<BenchmarkResultModel> benchmarkResults;

		_model.SetCurrentRunningBenchmark(_svgRasterizationSingle->GetName());

		_svgRasterizationSingle->GetConfigurations()[0].SetValue("./assets/svg-rasterization.single.svg");
		_svgRasterizationSingle->GetConfigurations()[1].SetValue("1.0");
		_svgRasterizationSingle->GetConfigurations()[2].SetValue("16");

		try
		{
			auto svgRasterizationSingle = co_await AsyncPromise<BenchmarkResultModel>([&]()
			{
				return _benchmarkExecutionService.Execute(
						*_svgRasterizationSingle,
						affinity,
						std::chrono::duration_cast<NanoSeconds>(
								_overheadsModel.GetNowOverhead()).count(),
						std::chrono::duration_cast<NanoSeconds>(
								_overheadsModel.GetLoopOverhead()).count(),
						std::chrono::duration_cast<NanoSeconds>(
								_overheadsModel.GetVirtualCallOverhead()).count(),
								1.0,
								1.0,
								false,
								false);
			});

			auto& taskResults = svgRasterizationSingle.GetTaskResults();

			auto& rasterizationResult = taskResults[0];
			singleCoreScore += rasterizationResult.GetInputSize() / rasterizationResult.GetDuration().count() / Divider;

			benchmarkResults.push_back(std::move(svgRasterizationSingle));
		}
		catch (const ElpidaException&)
		{
			_model.SetRunning(false);
			co_return;
		}

		_model.SetCurrentRunningBenchmark(_svgRasterizationMulti->GetName());

		auto targetSamples = _overheadsModel.GetIterationsPerSecond() * 16 / std::giga::num;

		_svgRasterizationMulti->GetConfigurations()[0].SetValue("./assets/svg-rasterization.multi.svg");
		_svgRasterizationMulti->GetConfigurations()[1].SetValue("1.0");
		_svgRasterizationMulti->GetConfigurations()[2].SetValue(std::to_string(targetSamples));

		try
		{
			auto svgRasterizationMulti = co_await AsyncPromise<BenchmarkResultModel>([&]()
			{
				return _benchmarkExecutionService.Execute(
						*_svgRasterizationMulti,
						affinity,
						std::chrono::duration_cast<NanoSeconds>(
								_overheadsModel.GetNowOverhead()).count(),
						std::chrono::duration_cast<NanoSeconds>(
								_overheadsModel.GetLoopOverhead()).count(),
						std::chrono::duration_cast<NanoSeconds>(
								_overheadsModel.GetVirtualCallOverhead()).count(),
						20.0,
						20.0,
						false,
						false);
			});

			auto& taskResults = svgRasterizationMulti.GetTaskResults();

			auto& rasterizationResult = taskResults[0];
			multiCoreScore += rasterizationResult.GetInputSize() / rasterizationResult.GetDuration().count() / Divider;

			benchmarkResults.push_back(std::move(svgRasterizationMulti));
		}
		catch (const ElpidaException&)
		{
			_model.SetRunning(false);
			co_return;
		}

		_model.SetCurrentRunningBenchmark(_memoryLatency->GetName());

		try
		{
			auto latencyResult = co_await AsyncPromise<BenchmarkResultModel>([&]()
			{
				return _benchmarkExecutionService.Execute(
						*_memoryLatency,
						affinity,
						std::chrono::duration_cast<NanoSeconds>(
								_overheadsModel.GetNowOverhead()).count(),
						std::chrono::duration_cast<NanoSeconds>(
								_overheadsModel.GetLoopOverhead()).count(),
						std::chrono::duration_cast<NanoSeconds>(
								_overheadsModel.GetVirtualCallOverhead()).count(),
						1.0,
						1.0,
					false,
					false);
			});

			auto& taskResults = latencyResult.GetTaskResults();

			memoryScore += Divider / (latencyResult.GetScore() * std::nano::den);

			benchmarkResults.push_back(std::move(latencyResult));
		}
		catch (const ElpidaException&)
		{
			_model.SetRunning(false);
			co_return;
		}


		_model.SetCurrentRunningBenchmark(_memoryReadBandwidth->GetName());

		try
		{
			auto bandwidthResult = co_await AsyncPromise<BenchmarkResultModel>([&]()
			{
				return _benchmarkExecutionService.Execute(
						*_memoryReadBandwidth,
						affinity,
						std::chrono::duration_cast<NanoSeconds>(
								_overheadsModel.GetNowOverhead()).count(),
						std::chrono::duration_cast<NanoSeconds>(
								_overheadsModel.GetLoopOverhead()).count(),
						std::chrono::duration_cast<NanoSeconds>(
								_overheadsModel.GetVirtualCallOverhead()).count(),
						1.0,
						1.0,
						false,
						false);
			});


			memoryScore += bandwidthResult.GetScore() / Divider / 1000;

			benchmarkResults.push_back(std::move(bandwidthResult));
		}
		catch (const ElpidaException&)
		{
			_model.SetRunning(false);
			co_return;
		}

		FullBenchmarkResultModel resultModel(std::move(benchmarkResults),
				singleCoreScore + multiCoreScore + memoryScore, singleCoreScore, multiCoreScore, memoryScore);

		_model.AddResult(std::move(resultModel));
		_model.SetRunning(false);
	}

	void FullBenchmarkController::StopRunning()
	{
		_cancelling = true;
		_benchmarkExecutionService.StopCurrentExecution();
	}
} // Elpida
// Application