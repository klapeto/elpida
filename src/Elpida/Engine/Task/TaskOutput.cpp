//
// Created by klapeto on 26/4/20.
//

#include "Elpida/Engine/Task/TaskOutput.hpp"
#include "Elpida/Engine/Data/PassiveTaskData.hpp"

namespace Elpida
{

	TaskOutput::TaskOutput()
		: _taskData(nullptr)
	{

	}

	TaskOutput::TaskOutput(RawData& data)
		: _taskData(&data)
	{
	}

	TaskOutput::~TaskOutput()
	{
		delete _taskData;
	}

	TaskOutput::TaskOutput(TaskOutput&& other) noexcept
	{
		delete _taskData;
		_taskData = other._taskData;
		other._taskData = nullptr;
	}

	TaskOutput& TaskOutput::operator=(TaskOutput&& other) noexcept
	{
		delete _taskData;
		_taskData = other._taskData;
		other._taskData = nullptr;
		return *this;
	}

}