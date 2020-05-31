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
// Created by klapeto on 19/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_RESULT_TASKRESULT_HPP
#define INCLUDE_ELPIDA_ENGINE_RESULT_TASKRESULT_HPP

#include <string>
#include "TaskMetrics.hpp"

namespace Elpida
{
	class TaskSpecification;
	class ResultSpecification;

	class TaskResult final
	{
	public:
		[[nodiscard]] const TaskSpecification& getTaskSpecification() const
		{
			return *_taskSpecification;
		}

		[[nodiscard]] const TaskMetrics& getMetrics() const
		{
			return _metrics;
		}

		TaskResult(const TaskSpecification& specification, TaskMetrics&& metrics);
		TaskResult(TaskResult&&) = default;
		TaskResult& operator=(TaskResult&&) = default;
		TaskResult(const TaskResult&) = default;
		TaskResult& operator=(const TaskResult&) = default;
		~TaskResult() = default;
	private:
		TaskMetrics _metrics;
		const TaskSpecification* _taskSpecification;
	};
}

#endif //INCLUDE_ELPIDA_ENGINE_RESULT_TASKRESULT_HPP
