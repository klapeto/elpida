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

#include "Elpida/Engine/Calculators/Task/ExclusiveHarmonicMeanTaskResultCalculator.hpp"

#include "Elpida/Config.hpp"
#include "Elpida/Common/ElpidaException.hpp"
#include "Elpida/Utilities/Statistics.hpp"

#include <iostream>
#include <algorithm>
#include <cmath>

namespace Elpida
{

	ProcessedTaskResult ExclusiveHarmonicMeanTaskResultCalculator::calculateAggregateResult(const std::vector<TaskResult>& taskResults) const
	{
		if (!taskResults.empty())
		{
			const auto& sampleResult = taskResults.front();

			auto preprocessedResults = taskResults;

			if (preprocessedResults.size() > _ignoreFirstResults)
			{
				preprocessedResults.erase(preprocessedResults.begin(), preprocessedResults.begin() + _ignoreFirstResults);
			}

			auto statistics = Statistics::calculateBasicStatistics(preprocessedResults, [](const TaskResult& x){return x.getMetrics().getDuration().count();});

			auto tTau = statistics.tau * statistics.standardDeviation;

			double totalDuration = 0.0;
			double totalValue = 0.0;
			size_t totalCount = 0;

			std::vector<TaskMetrics> outliers;

			for (auto& result : preprocessedResults)
			{
				auto delta = std::abs(result.getMetrics().getDuration().count() - statistics.mean);
				if (delta > tTau)
				{
					outliers.push_back(result.getMetrics());
					continue;
				}

				totalDuration += 1.0 / result.getMetrics().getDuration().count();
				totalValue += 1.0 / result.getMetrics().getResultValue();
				totalCount++;
			}

			return ProcessedTaskResult(TaskMetrics(Duration(totalCount / totalDuration),
				totalCount / totalValue,
				sampleResult.getMetrics().getInputDataSize()),
				statistics,
				std::move(outliers),
				sampleResult.getTaskSpecification());
		}
		else
		{
			throw ElpidaException(FUNCTION_NAME, "Attempted to calculate Task Results with empty result collection");
		}
	}
}