//
// Created by klapeto on 19/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_RESULT_BENCHMARKRESULT_HPP
#define INCLUDE_ELPIDA_ENGINE_RESULT_BENCHMARKRESULT_HPP


#include "TaskResult.hpp"
#include <vector>

namespace Elpida
{

	class Benchmark;

	class BenchmarkResult
	{
	public:
		using Score = size_t;

		[[nodiscard]] const Benchmark& getBenchmark() const
		{
			return _benchmark;
		}

		[[nodiscard]] const std::vector<TaskResult>& getTaskResults() const
		{
			return _taskResults;
		}

		BenchmarkResult(const Benchmark& benchmark, std::vector<TaskResult>&& taskResults, Score score)
			: _benchmark(benchmark), _taskResults(std::move(taskResults)), _score(score)
		{

		}
	private:
		const Benchmark& _benchmark;
		std::vector<TaskResult> _taskResults;
		Score _score;
	};
}

#endif //INCLUDE_ELPIDA_ENGINE_RESULT_BENCHMARKRESULT_HPP
