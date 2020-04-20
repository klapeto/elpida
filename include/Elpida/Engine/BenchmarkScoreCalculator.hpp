//
// Created by klapeto on 19/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_BENCHMARKSCORECALCULATOR_HPP
#define INCLUDE_ELPIDA_ENGINE_BENCHMARKSCORECALCULATOR_HPP

#include <cstddef>
#include <vector>
#include "Result/BenchmarkResult.hpp"

namespace Elpida
{
	class BenchmarkScoreCalculator
	{
	public:

		[[nodiscard]] virtual BenchmarkResult::Score calculate(const std::vector<TaskResult>& taskResults) const = 0;

		BenchmarkScoreCalculator() = default;
		virtual ~BenchmarkScoreCalculator() = default;
	};
}


#endif //INCLUDE_ELPIDA_ENGINE_BENCHMARKSCORECALCULATOR_HPP
