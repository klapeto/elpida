//
// Created by klapeto on 26/4/20.
//

#include "Elpida/Engine/Task/TaskInput.hpp"
#include "Elpida/Engine/Task/Data/TaskData.hpp"

namespace Elpida
{

	TaskInput::~TaskInput()
	{
		for (auto data: _taskData)
		{
			delete data;
		}
	}
}