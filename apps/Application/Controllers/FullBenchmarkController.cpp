//
// Created by klapeto on 22/10/2023.
//

#include "FullBenchmarkController.hpp"

#include "Models/SystemInfo/TopologyModel.hpp"
#include "Models/SystemInfo/TimingModel.hpp"

#include "Core/BenchmarkExecutionService.hpp"

namespace Elpida::Application
{
	FullBenchmarkController::FullBenchmarkController(FullBenchmarkModel& model,
			const TopologyModel& topologyModel,
			const TimingModel& overheadsModel,
			BenchmarkExecutionService& benchmarkExecutionService,
			const std::vector<BenchmarkGroupModel>& benchmarkGroups) :
			Controller(model),
			_topologyModel(topologyModel),
			_overheadsModel(overheadsModel),
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

		std::vector<BenchmarkResultModel> benchmarkResults;

		_model.SetCurrentRunningBenchmark(_memoryLatency->GetName());

		try
		{
			auto pngResult = co_await AsyncPromise<BenchmarkResultModel>([&]()
			{
				return _benchmarkExecutionService.Execute(
						*_memoryLatency,
						affinity,
						std::chrono::duration_cast<NanoSeconds>(
								_overheadsModel.GetNowOverhead()).count(),
						std::chrono::duration_cast<NanoSeconds>(
								_overheadsModel.GetLoopOverhead()).count(),
						std::chrono::duration_cast<NanoSeconds>(
								_overheadsModel.GetVirtualCallOverhead()).count());
			});
			benchmarkResults.push_back(std::move(pngResult));
		}
		catch (const ElpidaException&)
		{
			_model.SetRunning(false);
			co_return;
		}

		FullBenchmarkResultModel resultModel(std::move(benchmarkResults), 1000, 5461313, 5464213, 132132);

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