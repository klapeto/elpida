//
// Created by klapeto on 19/4/20.
//

#include "Elpida/Engine/DefaultBenchmarkScoreCalculator.hpp"
namespace Elpida
{

	BenchmarkResult::Score DefaultBenchmarkScoreCalculator::calculate(const std::vector<TaskResult>& taskResults) const
	{
		auto accumulator = 0ul;

		for (auto& res: taskResults)
		{
			accumulator += (double)res.getMetrics().getResultValue();
		}

		return accumulator;
	}
}