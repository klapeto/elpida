//
// Created by klapeto on 11/5/2024.
//

#ifndef ELPIDA_FULLBENCHMARKINSTANCE_HPP
#define ELPIDA_FULLBENCHMARKINSTANCE_HPP

#include "Core/BenchmarkScore.hpp"
#include "Models/Benchmark/BenchmarkResultModel.hpp"

namespace Elpida::Application
{

	class FullBenchmarkInstanceResult
	{
	public:
		BenchmarkResultModel& GetBenchmarkResult();
		Score GetSingleCoreScore() const;
		Score GetMultiThreadScore() const;
		Score GetMemoryScore() const;
		FullBenchmarkInstanceResult(BenchmarkResultModel&& benchmarkResult, Score singleCoreScore,
				Score multiThreadScore, Score memoryScore);
	private:
		BenchmarkResultModel _benchmarkResult;
		Score _singleCoreScore;
		Score _multiThreadScore;
		Score _memoryScore;
	};

	class BenchmarkModel;
	class TimingModel;
	class TopologyModel;
	class BenchmarkExecutionService;

	class FullBenchmarkInstance
	{
	public:
		[[nodiscard]]
		virtual std::string GetName() const = 0;

		[[nodiscard]]
		virtual FullBenchmarkInstanceResult Run() = 0;

		virtual ~FullBenchmarkInstance() = default;

	protected:
		FullBenchmarkInstance(const BenchmarkModel& benchmark,
				const TimingModel& timingModel,
				const TopologyModel& topologyModel,
				BenchmarkExecutionService& executionService);

		const BenchmarkModel& _benchmark;
		const TimingModel& _timingModel;
		const TopologyModel& _topologyModel;
		BenchmarkExecutionService& _executionService;

		const double Divider = 1000000.0;
	};

} // Application
// Elpida

#endif //ELPIDA_FULLBENCHMARKINSTANCE_HPP
