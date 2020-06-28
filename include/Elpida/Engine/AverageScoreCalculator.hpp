/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2020  Ioannis Panagiotopoulos
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>
 *************************************************************************/

//
// Created by klapeto on 28/6/20.
//

#ifndef SRC_ELPIDA_ENGINE_AVERAGESCORECALCULATOR_HPP
#define SRC_ELPIDA_ENGINE_AVERAGESCORECALCULATOR_HPP

#include "BenchmarkScoreCalculator.hpp"

namespace Elpida
{
	class AverageScoreCalculator : public BenchmarkScoreCalculator
	{
	public:
		[[nodiscard]] BenchmarkResult::Score calculate(const std::vector<TaskResult>& taskResults) const override;

		AverageScoreCalculator() = default;
		explicit AverageScoreCalculator(const std::string& suffix);
		~AverageScoreCalculator() override = default;
	};
}


#endif //SRC_ELPIDA_ENGINE_AVERAGESCORECALCULATOR_HPP
