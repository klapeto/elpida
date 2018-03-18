/**************************************************************************
*   elpida - CPU benchmark tool
*   
*   Copyright (C) 2018  Ioannis Panagiotopoulos
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

/*
 * MultiThreadTask.cpp
 *
 *  Created on: 17 Μαρ 2018
 *      Author: klapeto
 */

#include "MultiThreadTask.hpp"
#include "CpuInfo.hpp"

namespace Elpida
{

	MultiThreadTask::MultiThreadTask(const std::vector<Task*>& tasks, bool strictAffinity) :
			Task(tasks[0]->getName() + " (Multi Threaded)"), _strictAffinity(strictAffinity)
	{
		if (tasks.size() > (size_t) CpuInfo::getCpuInfo().getLogicalProcessors())
		{
			_strictAffinity = false;
		}
		int affinity = 0;
		for (auto task : tasks)
		{
			_tasksToBeExecuted.push_back(TaskThread(*task, _mutex, _strictAffinity ? affinity++ : -1));
		}
	}

	MultiThreadTask::~MultiThreadTask()
	{
	}

} /* namespace Elpida */
