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
// Created by klapeto on 14/11/20.
//

#include "Elpida/Engine/Calculators/Task/AverageTaskResultCalculator.hpp"

#include "Elpida/Config.hpp"
#include "Elpida/ElpidaException.hpp"

namespace Elpida
{

	TaskResult AverageTaskResultCalculator::calculateAggregateResult(const std::vector<TaskResult>& taskResults) const
	{
		if (!taskResults.empty())
		{
			const auto& sampleResult = taskResults.front();

			Duration totalDuration;
			double totalValue = 0.0;

			for (auto& result: taskResults)
			{
				totalDuration += result.getMetrics().getDuration();
				totalValue += result.getMetrics().getResultValue();
			}

			return TaskResult(sampleResult.getTaskSpecification(),
				Elpida::TaskMetrics(totalDuration / taskResults.size(),
					totalValue / taskResults.size(),
					sampleResult.getMetrics().getInputDataSize()));
		}
		else
		{
			throw ElpidaException(FUNCTION_NAME, "Attempted to calculate Task Results with empty result collection");
		}

	}
}