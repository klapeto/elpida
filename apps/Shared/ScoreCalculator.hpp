//
// Created by klapeto on 12/9/26.
//

#ifndef ELPIDA_SCORECALCULATOR_HPP
#define ELPIDA_SCORECALCULATOR_HPP
#include <cstddef>

namespace Elpida
{
	class ScoreCalculator
	{
	public:
		static double CalculateTotalScore(double singleCoreScore, double multiCoreScore);
		static double CalculateBenchmarkScore(const double score[], const double baseScores[], std::size_t size);
	};
} // Elpida

#endif //ELPIDA_SCORECALCULATOR_HPP
