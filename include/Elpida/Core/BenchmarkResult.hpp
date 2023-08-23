//
// Created by klapeto on 27/2/2023.
//

#ifndef ELPIDA_BENCHMARKRESULT_HPP
#define ELPIDA_BENCHMARKRESULT_HPP

#include "Elpida/Core/Vector.hpp"
#include "Elpida/Core/String.hpp"
#include "Elpida/Core/TaskResult.hpp"
#include "BenchmarkInfo.hpp"

namespace Elpida
{
	class BenchmarkResult final
	{
	 public:

		[[nodiscard]]
		const BenchmarkInfo& GetBenchmarkInfo() const
		{
			return _benchmarkInfo;
		}

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

		BenchmarkResult(BenchmarkInfo&& benchmarkInfo, double score, Vector<TaskResult>&& taskResults)
			: _benchmarkInfo(std::move(benchmarkInfo)), _taskResults(std::move(taskResults)), _score(score)
		{
		}

		~BenchmarkResult() = default;
	 private:
		BenchmarkInfo _benchmarkInfo;
		Vector<TaskResult> _taskResults;
		double _score;
	};

} // Elpida

#endif //ELPIDA_BENCHMARKRESULT_HPP
