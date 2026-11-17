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
	// Geometric mean of the single and multi-thread scores: a scale factor or other base scores multiply every total by
	// the same constant, so rankings don't change. The multi-thread score already contains the per-core speed
	// (multi ≈ single × scaling), so the total is single × sqrt(scaling): doubling the single-thread speed doubles it,
	// doubling the cores adds about 41%.
	double ScoreCalculator::CalculateTotalScore(double singleCoreScore, double multiCoreScore)
	{
		return std::sqrt(singleCoreScore * multiCoreScore);
	}

	// Geometric mean of the score / base score ratios. The base scores then only set the scale: how two machines
	// compare doesn't depend on which machine the base scores come from (with the harmonic mean used before, it did).
	double ScoreCalculator::CalculateBenchmarkScore(const double score[], const double baseScores[], const std::size_t size)
	{
		if (size == 0) return 0.0;

		double logSum = 0.0;
		for (std::size_t i = 0; i < size; ++i)
		{
			logSum += std::log(score[i] / baseScores[i]);
		}
		return std::exp(logSum / static_cast<double>(size));
	}

} // Elpida