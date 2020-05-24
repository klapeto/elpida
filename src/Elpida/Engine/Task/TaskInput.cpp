//
// Created by klapeto on 26/4/20.
//

#include "Elpida/Engine/Task/TaskInput.hpp"
#include "Elpida/Utilities/RawData.hpp"
#include "Elpida/Engine/Data/PassiveTaskData.hpp"

namespace Elpida
{

	TaskInput::TaskInput()
		: _taskData(nullptr)
	{

	}

	TaskInput::TaskInput(RawData& taskData)
		: _taskData(&taskData)
	{
	}

	TaskInput::~TaskInput()
	{
		delete _taskData;
	}

	TaskInput::TaskInput(TaskInput&& other) noexcept
	{
		_taskData = other._taskData;
		other._taskData = nullptr;
	}

	TaskInput& TaskInput::operator=(TaskInput&& other) noexcept
	{
		_taskData = other._taskData;
		other._taskData = nullptr;
		return *this;
	}

}