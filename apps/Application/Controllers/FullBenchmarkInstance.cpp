//
// Created by klapeto on 11/5/2024.
//

#include "FullBenchmarkInstance.hpp"

namespace Elpida::Application
{

	FullBenchmarkInstanceResult::FullBenchmarkInstanceResult(BenchmarkResultModel&& benchmarkResult,
			Score singleCoreScore, Score multiThreadScore, Score memoryScore)
			:_benchmarkResult(std::move(benchmarkResult)), _singleCoreScore(singleCoreScore), _multiThreadScore(multiThreadScore),
			 _memoryScore(memoryScore)
	{
	}

	BenchmarkResultModel& FullBenchmarkInstanceResult::GetBenchmarkResult()
	{
		return _benchmarkResult;
	}

	Score FullBenchmarkInstanceResult::GetSingleCoreScore() const
	{
		return _singleCoreScore;
	}

	Score FullBenchmarkInstanceResult::GetMultiThreadScore() const
	{
		return _multiThreadScore;
	}

	Score FullBenchmarkInstanceResult::GetMemoryScore() const
	{
		return _memoryScore;
	}

	FullBenchmarkInstance::FullBenchmarkInstance(const BenchmarkModel& benchmark,
			const TimingModel& timingModel,
			const TopologyModel& topologyModel,
			BenchmarkExecutionService& executionService)
			:_benchmark(benchmark),
			 _timingModel(timingModel),
			 _topologyModel(topologyModel),
			 _executionService(executionService)
	{
	}
} // Application
// Elpida