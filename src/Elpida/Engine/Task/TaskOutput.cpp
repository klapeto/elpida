//
// Created by klapeto on 26/4/20.
//

#include "Elpida/Engine/Task/TaskOutput.hpp"
#include "Elpida/Engine/Data/PassiveTaskData.hpp"

namespace Elpida
{

	TaskOutput::TaskOutput(TaskData* data)
	{
		_taskData.push_back(data);
	}

	TaskOutput::TaskOutput(std::vector<TaskData*>&& taskData)
		: _taskData(std::move(taskData))
	{
	}

	TaskOutput::~TaskOutput()
	{
		for (auto data: _taskData)
		{
			delete data;
		}
	}

	std::vector<TaskData*> TaskOutput::createPassiveWrappers() const
	{
		std::vector<TaskData*> data;
		for (auto& d : _taskData)
		{
			data.push_back(new PassiveTaskData(*d));
		}
		return data;
	}
}