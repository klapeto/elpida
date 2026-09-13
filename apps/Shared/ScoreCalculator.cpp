//
// Created by klapeto on 12/9/26.
//

#include "ScoreCalculator.hpp"
#include <cmath>

namespace Elpida
{
	static constexpr double SingleCoreWeight = 1.1;
	static constexpr double MultiCoreWeight = 1.0;

	double ScoreCalculator::CalculateTotalScore(double singleCoreScore, double multiCoreScore)
	{
		return std::pow(singleCoreScore, SingleCoreWeight) + std::pow(multiCoreScore, MultiCoreWeight);
	}

	double ScoreCalculator::CalculateBenchmarkScore(const double score[], const double baseScores[], const std::size_t size)
	{
		double totalScore = 0.0;
		for (std::size_t i = 0; i < size; ++i)
		{
			const auto thisScore = score[i];
			const auto thisBaseScore = baseScores[i];
			totalScore += 1.0 / (thisScore / thisBaseScore);
		}
		return size / totalScore;
	}

} // Elpida