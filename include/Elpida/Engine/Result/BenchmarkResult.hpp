//
// Created by klapeto on 19/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_RESULT_BENCHMARKRESULT_HPP
#define INCLUDE_ELPIDA_ENGINE_RESULT_BENCHMARKRESULT_HPP

#include "TaskResult.hpp"
#include <vector>
#include <string>

namespace Elpida
{
	class Benchmark;

	class BenchmarkResult
	{
	public:
		using Score = size_t;

		[[nodiscard]] const Benchmark& getBenchmark() const
		{
			return *_benchmark;
		}

		[[nodiscard]] const std::vector<TaskResult>& getTaskResults() const
		{
			return _taskResults;
		}

		[[nodiscard]] const std::string& getId() const
		{
			return _id;
		}

		[[nodiscard]] Score getScore() const
		{
			return _score;
		}

		BenchmarkResult(const Benchmark& benchmark, std::vector<TaskResult>&& taskResults, Score score);
		BenchmarkResult(BenchmarkResult&&) = default;
		BenchmarkResult& operator=(BenchmarkResult&&) = default;
		BenchmarkResult(const BenchmarkResult&) = default;
		BenchmarkResult& operator=(const BenchmarkResult&) = default;
		~BenchmarkResult() = default;
	private:
		const Benchmark* _benchmark;
		std::vector<TaskResult> _taskResults;
		std::string _id;
		Score _score;
	};
}

#endif //INCLUDE_ELPIDA_ENGINE_RESULT_BENCHMARKRESULT_HPP
