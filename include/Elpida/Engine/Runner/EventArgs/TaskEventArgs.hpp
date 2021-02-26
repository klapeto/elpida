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

#ifndef INCLUDE_ELPIDA_ENGINE_RUNNER_EVENTARGS_TASKEVENTARGS_HPP
#define INCLUDE_ELPIDA_ENGINE_RUNNER_EVENTARGS_TASKEVENTARGS_HPP

#include <unistd.h>
#include <string>

namespace Elpida
{
	class TaskSpecification;
	class TaskBuilder;

	class TaskEventArgs
	{
	public:

		[[nodiscard]]
		const TaskBuilder& getTaskBuilder() const
		{
			return _taskBuilder;
		}

		[[nodiscard]]
		size_t getIteration() const
		{
			return _iteration;
		}

		explicit TaskEventArgs(const TaskBuilder& taskBuilder, size_t iteration = 1);
		virtual ~TaskEventArgs() = default;
	private:
		const TaskBuilder& _taskBuilder;
		size_t _iteration;
	};
}

#endif //INCLUDE_ELPIDA_ENGINE_RUNNER_EVENTARGS_TASKEVENTARGS_HPP
