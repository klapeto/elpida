/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2018  Ioannis Panagiotopoulos
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

/*
 * TaskThroughput.hpp
 *
 *  Created on: 18 Μαρ 2018
 *      Author: klapeto
 */

#ifndef ELPIDA_TASKTHROUGHPUT_HPP_
#define ELPIDA_TASKTHROUGHPUT_HPP_

#include "Elpida/Types/String.hpp"
#include "Elpida/TaskMetrics.hpp"
#include "Elpida/TaskRunResult.hpp"
#include "Elpida/Utilities/ValueUtilities.hpp"

namespace Elpida
{

	class TaskThroughput
	{
		public:

			String getPerSecond() const
			{
				return ValueUtilities::getValueScale(_runResult.getMeasuredValue() / _taskMetrics.getSubdivision<TaskMetrics::Second>()) + _runResult.getMeasuredValueName() + "/s";
			}

			const TaskRunResult& getRunResult() const
			{
				return _runResult;
			}

			const TaskMetrics& getTaskMetrics() const
			{
				return _taskMetrics;
			}

			TaskThroughput(const TaskRunResult& runResult, const TaskMetrics& taskMetrics)
					: _runResult(runResult), _taskMetrics(taskMetrics)
			{

			}
			virtual ~TaskThroughput()
			{

			}

			TaskThroughput(TaskThroughput&&) = default;
			TaskThroughput(const TaskThroughput&) = default;
			TaskThroughput& operator=(TaskThroughput&&) = default;
			TaskThroughput& operator=(const TaskThroughput&) = default;
		private:
			TaskRunResult _runResult;
			TaskMetrics _taskMetrics;

	};

} /* namespace Elpida */

#endif /* ELPIDA_TASKTHROUGHPUT_HPP_ */
