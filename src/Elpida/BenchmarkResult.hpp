//
// Created by klapeto on 27/2/2023.
//

#ifndef ELPIDA_BENCHMARKRESULT_HPP
#define ELPIDA_BENCHMARKRESULT_HPP

#include <utility>
#include <vector>
#include <string>

#include "Elpida/Duration.hpp"
#include "Elpida/TaskResult.hpp"
#include "Elpida/ScoreType.hpp"

namespace Elpida
{
	class BenchmarkResult
	{
	 public:
		double GetScore() const
		{
			return _score;
		}

		const std::vector<TaskResult>& GetTaskResults() const
		{
			return _taskResults;
		}

		BenchmarkResult(double score, const std::vector<TaskResult>& taskResults)
			: _taskResults(taskResults), _score(score)
		{
		}

		~BenchmarkResult() = default;
	 private:
		std::vector<TaskResult> _taskResults;
		double _score;
	};

} // Elpida

#endif //ELPIDA_BENCHMARKRESULT_HPP
