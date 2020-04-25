//
// Created by klapeto on 19/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_DEFAULTBENCHMARKSCORECALCULATOR_HPP
#define INCLUDE_ELPIDA_ENGINE_DEFAULTBENCHMARKSCORECALCULATOR_HPP


#include "BenchmarkScoreCalculator.hpp"
namespace Elpida {
	class DefaultBenchmarkScoreCalculator final: public BenchmarkScoreCalculator
	{
	public:

		BenchmarkResult::Score calculate(const std::vector<TaskResult> &taskResults) const override;

		DefaultBenchmarkScoreCalculator() =default;
		~DefaultBenchmarkScoreCalculator() override  =default;
	};
}

#endif //INCLUDE_ELPIDA_ENGINE_DEFAULTBENCHMARKSCORECALCULATOR_HPP
