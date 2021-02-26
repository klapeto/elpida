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
// Created by klapeto on 13/2/21.
//

#ifndef INCLUDE_ELPIDA_ENGINE_RESULT_PROCESSEDTASKRESULT_HPP
#define INCLUDE_ELPIDA_ENGINE_RESULT_PROCESSEDTASKRESULT_HPP

#include <vector>

#include "TaskMetrics.hpp"
#include "Elpida/Utilities/Statistics.hpp"

namespace Elpida
{
	class TaskSpecification;

	class ProcessedTaskResult final
	{
	public:

		[[nodiscard]]
		const TaskMetrics& getFinalMetrics() const
		{
			return _finalMetrics;
		}

		[[nodiscard]]
		const BasicStatistics& getBasicStatistics() const
		{
			return _basicStatistics;
		}

		[[nodiscard]]
		const std::vector<TaskMetrics>& getOutliers() const
		{
			return _outliers;
		}

		[[nodiscard]]
		const TaskSpecification& getTaskSpecification() const
		{
			return _taskSpecification;
		}

		ProcessedTaskResult(const TaskMetrics& finalMetrics,
			const BasicStatistics& basicStatistics,
			std::vector<TaskMetrics>&& outliers,
			const TaskSpecification& taskSpecification)
			: _finalMetrics(finalMetrics),
			  _basicStatistics(basicStatistics),
			  _outliers(outliers),
			  _taskSpecification(taskSpecification)
		{
		}

		~ProcessedTaskResult() = default;
	private:
		TaskMetrics _finalMetrics;
		BasicStatistics _basicStatistics;
		std::vector<TaskMetrics> _outliers;

		const TaskSpecification& _taskSpecification;
	};
}

#endif //INCLUDE_ELPIDA_ENGINE_RESULT_PROCESSEDTASKRESULT_HPP
