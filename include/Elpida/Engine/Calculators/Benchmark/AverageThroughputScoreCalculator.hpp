/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2021 Ioannis Panagiotopoulos
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
// Created by klapeto on 3/5/21.
//

#ifndef ELPIDA_AVERAGETHROUGHPUTSCORECALCULATOR_HPP
#define ELPIDA_AVERAGETHROUGHPUTSCORECALCULATOR_HPP

#include "Elpida/Engine/Calculators/BenchmarkScoreCalculator.hpp"

namespace Elpida
{
	class AverageThroughputScoreCalculator : public BenchmarkScoreCalculator
	{
	public:
		[[nodiscard]]
		BenchmarkScore
		calculate(const Benchmark& benchmark, const std::vector<ProcessedTaskResult>& taskResults) const override;

		AverageThroughputScoreCalculator() = default;

		~AverageThroughputScoreCalculator() override = default;
	};
}

#endif //ELPIDA_AVERAGETHROUGHPUTSCORECALCULATOR_HPP
