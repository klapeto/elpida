//
// Created by klapeto on 26/4/20.
//

#include "Elpida/Engine/Task/TaskDataDto.hpp"
#include "Elpida/Utilities/RawData.hpp"
#include "Elpida/Engine/Data/PassiveTaskData.hpp"

namespace Elpida
{

	TaskDataDto::TaskDataDto(RawData& taskData, std::unordered_map<std::string, double> properties)
		: _definedProperties(std::move(properties)), _taskData(&taskData)
	{

	}

	TaskDataDto::TaskDataDto()
		: _taskData(nullptr)
	{

	}

	TaskDataDto::TaskDataDto(RawData& taskData)
		: _taskData(&taskData)
	{
	}

}