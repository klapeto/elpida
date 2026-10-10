////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2026 Ioannis Panagiotopoulos
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////////////

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