//
// Created by klapeto on 27/2/2023.
//

#ifndef ELPIDA_BENCHMARKRESULT_HPP
#define ELPIDA_BENCHMARKRESULT_HPP

#include "Elpida/Vector.hpp"
#include "Elpida/String.hpp"
#include "Elpida/TaskResult.hpp"

namespace Elpida
{
	class BenchmarkResult
	{
	 public:

		[[nodiscard]]
		double GetScore() const
		{
			return _score;
		}

		[[nodiscard]]
		const Vector<TaskResult>& GetTaskResults() const
		{
			return _taskResults;
		}

		BenchmarkResult(double score, const Vector<TaskResult>& taskResults)
			: _taskResults(taskResults), _score(score)
		{
		}

		~BenchmarkResult() = default;
	 private:
		Vector<TaskResult> _taskResults;
		double _score;
	};

} // Elpida

#endif //ELPIDA_BENCHMARKRESULT_HPP
