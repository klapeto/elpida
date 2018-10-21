/**************************************************************************
 *   Elpida - Benchmark library
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

#include "Elpida/MultiThreadTask.hpp"

#include "Elpida/CpuInfo.hpp"

namespace Elpida
{

	MultiThreadTask::MultiThreadTask(const String& name, bool strictAffinity)
			: Task(name + "(Multi Threaded)"), _threadsShouldWake(false), _strictAffinity(strictAffinity)
	{
	}

	MultiThreadTask::~MultiThreadTask()
	{
		destroyTasks();
	}

	void MultiThreadTask::addTask(Task* task)
	{
		static Size cores = CpuInfo::getCpuInfo().getLogicalProcessors();

		if (_tasksToBeExecuted.size() == cores)
		{
			_strictAffinity = false;
			for (auto& task : _tasksToBeExecuted)
			{
				task.setAffinity(-1);
			}
		}
		_tasksToBeExecuted.push_back(TaskThread(*task,_wakeNotifier, _mutex, _threadsShouldWake, _strictAffinity ? _tasksToBeExecuted.size() : -1));
		_createdTasks.push_back(task);
	}

	void MultiThreadTask::destroyTasks()
	{
		_tasksToBeExecuted.clear();
		for (auto task : _createdTasks)
		{
			delete task;
		}
		_createdTasks.clear();
	}

} /* namespace Elpida */
