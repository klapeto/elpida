//
// Created by klapeto on 26/4/20.
//

#include "Elpida/Engine/Task/TaskInput.hpp"
#include "Elpida/Engine/Data/TaskData.hpp"
#include "Elpida/Engine/Data/PassiveTaskData.hpp"

namespace Elpida
{

	TaskInput::~TaskInput()
	{
		for (auto data: _taskData)
		{
			delete data;
		}
	}

	std::vector<TaskData*> TaskInput::createPassiveWrappers() const
	{
		std::vector<TaskData*> returnVector;
		for (auto data: _taskData)
		{
			returnVector.push_back(new PassiveTaskData(data->getData(), data->getSize()));
		}
		return returnVector;
	}
}