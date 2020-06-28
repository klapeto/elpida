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

#include "Elpida/Engine/AverageScoreCalculator.hpp"

#include "Elpida/Engine/Result/TaskResult.hpp"
#include "Elpida/Engine/Result/ResultSpecification.hpp"
#include "Elpida/Engine/Task/TaskSpecification.hpp"
#include "Elpida/ElpidaException.hpp"
#include "Elpida/Config.hpp"

namespace Elpida
{

	BenchmarkResult::Score AverageScoreCalculator::calculate(const std::vector<TaskResult>& taskResults) const
	{
		if (!taskResults.empty())
		{
			BenchmarkResult::Score score = 0.0;
			for (const auto& result: taskResults)
			{
				score += result.getMetrics().getResultValue();
			}
			return score / (double)taskResults.size();
		}
		return 0;
	}

	AverageScoreCalculator::AverageScoreCalculator(const std::string& suffix)
		: BenchmarkScoreCalculator(suffix, ResultType::Raw)
	{

	}
}