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
