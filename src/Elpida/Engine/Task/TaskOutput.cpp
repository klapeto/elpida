//
// Created by klapeto on 26/4/20.
//

#include "Elpida/Engine/Task/TaskOutput.hpp"
namespace Elpida
{

	TaskOutput::~TaskOutput()
	{
		for (auto data: _taskData)
		{
			delete data;
		}
	}
}