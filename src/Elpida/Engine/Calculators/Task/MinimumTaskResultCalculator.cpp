/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2021  Ioannis Panagiotopoulos
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
// Created by klapeto on 7/2/21.
//

#include "Elpida/Engine/Calculators/Task/MinimumTaskResultCalculator.hpp"

#include "Elpida/Config.hpp"
#include "Elpida/Common/ElpidaException.hpp"
#include "Elpida/Utilities/Statistics.hpp"

namespace Elpida
{

	ProcessedTaskResult MinimumTaskResultCalculator::calculateAggregateResult(const std::vector<TaskResult>& taskResults) const
	{
		if (!taskResults.empty())
		{
			const auto& sampleResult = taskResults.front();

			double duration = std::numeric_limits<double>::max();
			double value = std::numeric_limits<double>::max();


			for (auto& result: taskResults)
			{
				duration = std::min(duration, result.getMetrics().getDuration().count());
				value = std::min(value, result.getMetrics().getResultValue());
			}

			return ProcessedTaskResult(TaskMetrics(Duration(duration),
				value,
				sampleResult.getMetrics().getInputDataSize()),
				Statistics::calculateBasicStatistics(taskResults, [](const TaskResult& x){return x.getMetrics().getDuration().count();}),
				std::vector<TaskMetrics>(),
				sampleResult.getTaskSpecification());
		}
		else
		{
			throw ElpidaException(FUNCTION_NAME, "Attempted to calculate Task Results with empty result collection");
		}
	}
}