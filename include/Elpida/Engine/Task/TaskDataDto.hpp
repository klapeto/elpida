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
#include <memory>

#include "Elpida/Engine/Data/RawTaskData.hpp"

namespace Elpida
{
	class TaskDataDto final
	{
	public:
		using Properties = std::unordered_map<std::string, double>;

		[[nodiscard]] const std::unique_ptr<RawTaskData>& getTaskData() const
		{
			return _taskData;
		}

		[[nodiscard]] std::unique_ptr<RawTaskData> getTaskDataAndRelease()
		{
			return std::move(_taskData);
		}

		[[nodiscard]] bool hasData() const
		{
			return _taskData.get();
		}

		void setTaskData(std::unique_ptr<RawTaskData> data)
		{
			_taskData = std::move(data);
		}

		void addProperties(std::initializer_list<Properties::value_type> pairs)
		{
			_definedProperties.insert(pairs);
		}

		void setProperties(Properties properties)
		{
			_definedProperties = std::move(properties);
		}

		const Properties& getDefinedProperties() const
		{
			return _definedProperties;
		}

		TaskDataDto() = default;
		explicit TaskDataDto(std::unique_ptr<RawTaskData> taskData);
		explicit TaskDataDto(std::unique_ptr<RawTaskData> taskData, std::initializer_list<Properties::value_type> properties);
		explicit TaskDataDto(std::unique_ptr<RawTaskData> taskData, Properties properties);
		TaskDataDto(const TaskDataDto&) = delete;
		TaskDataDto& operator=(const TaskDataDto&) = delete;
		TaskDataDto(TaskDataDto&& other) noexcept = default;
		TaskDataDto& operator=(TaskDataDto&& other) noexcept = default;
		~TaskDataDto() = default;
	private:
		Properties _definedProperties;
		std::unique_ptr<RawTaskData> _taskData;
	};
}

#endif //INCLUDE_ELPIDA_ENGINE_TASK_TASKDATADTO_HPP
