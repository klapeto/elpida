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
			_pngEncoding(nullptr),
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
				else if (name == "Png Encoding/Decoding")
				{
					_pngEncoding = &benchmark;
				}
			}
		}
		if (!_memoryReadBandwidth || !_memoryLatency)
		{
			throw ElpidaException("Missing benchmarks: Memory benchmarks");
		}

		if (!_pngEncoding)
		{
			throw ElpidaException("Missing benchmarks: Image benchmarks");
		}

		_model.SetTotalBenchmarks(3);
	}

	Promise<> FullBenchmarkController::RunAsync()
	{
		_cancelling = false;
		_model.SetRunning(true);
		std::vector<std::size_t> affinity;

		affinity.reserve(_topologyModel.GetLeafNodes().size());
		for (auto& node: _topologyModel.GetLeafNodes())
		{
			affinity.push_back(node.get().GetOsIndex().value());
		}

		FullBenchmarkResultModel::Score singleCoreScore = 0;
		FullBenchmarkResultModel::Score multiCoreScore = 0.0;
		FullBenchmarkResultModel::Score memoryScore = 0.0;
		std::vector<BenchmarkResultModel> benchmarkResults;

		_model.SetCurrentRunningBenchmark(_pngEncoding->GetName());

		_pngEncoding->GetConfigurations()[0].SetValue("/home/klapeto/Elpida-1.png");
		_pngEncoding->GetConfigurations()[1].SetValue("/home/klapeto/Elpida-1.out.png");
		try
		{
			auto pngResult = co_await AsyncPromise<BenchmarkResultModel>([&]()
			{
				return _benchmarkExecutionService.Execute(
						*_pngEncoding,
						affinity,
						std::chrono::duration_cast<NanoSeconds>(
								_overheadsModel.GetNowOverhead()).count(),
						std::chrono::duration_cast<NanoSeconds>(
								_overheadsModel.GetLoopOverhead()).count(),
						std::chrono::duration_cast<NanoSeconds>(
								_overheadsModel.GetVirtualCallOverhead()).count(),
								_benchmarkRunConfigurationModel.IsNumaAware());
			});

			auto& taskResults = pngResult.GetTaskResults();

			auto& decoding = taskResults[1];
			singleCoreScore += decoding.GetInputSize() / decoding.GetDuration().count() / Divider;

			auto& encoding = taskResults[5];
			singleCoreScore += encoding.GetInputSize() / encoding.GetDuration().count() / Divider;

			auto& grayscale = taskResults[3];
			multiCoreScore += grayscale.GetInputSize() / grayscale.GetDuration().count() / Divider / 10;

			benchmarkResults.push_back(std::move(pngResult));
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
					_benchmarkRunConfigurationModel.IsNumaAware());
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
					_benchmarkRunConfigurationModel.IsNumaAware());
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