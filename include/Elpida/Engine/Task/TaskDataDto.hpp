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
// Created by klapeto on 26/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_TASK_TASKDATADTO_HPP
#define INCLUDE_ELPIDA_ENGINE_TASK_TASKDATADTO_HPP

#include <unordered_map>
#include <string>

namespace Elpida
{
	class RawData;

	class TaskDataDto final
	{
	public:

		[[nodiscard]] RawData* getTaskData() const
		{
			return _taskData;
		}

		const std::unordered_map<std::string, double>& getDefinedProperties() const
		{
			return _definedProperties;
		}

		TaskDataDto();
		explicit TaskDataDto(RawData& taskData);
		explicit TaskDataDto(RawData& taskData, std::unordered_map<std::string, double> properties);
		TaskDataDto(const TaskDataDto&) = default;
		TaskDataDto& operator=(const TaskDataDto&) = default;
		TaskDataDto(TaskDataDto&& other) noexcept = default;
		TaskDataDto& operator=(TaskDataDto&& other) noexcept = default;
		~TaskDataDto() = default;
	private:
		std::unordered_map<std::string, double> _definedProperties;
		RawData* _taskData;
	};
}


#endif //INCLUDE_ELPIDA_ENGINE_TASK_TASKDATADTO_HPP
