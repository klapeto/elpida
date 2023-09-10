//
// Created by klapeto on 27/2/2023.
//

#ifndef ELPIDA_BENCHMARKRESULT_HPP
#define ELPIDA_BENCHMARKRESULT_HPP

#include "Elpida/Core/Vector.hpp"
#include "Elpida/Core/String.hpp"
#include "Elpida/Core/TaskResult.hpp"

namespace Elpida
{
	class BenchmarkResult final
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

		BenchmarkResult(double score, Vector<TaskResult>&& taskResults)
			: _taskResults(std::move(taskResults)), _score(score)
		{
		}

		~BenchmarkResult() = default;
	 private:
		Vector<TaskResult> _taskResults;
		double _score;
	};

} // Elpida

#endif //ELPIDA_BENCHMARKRESULT_HPP
