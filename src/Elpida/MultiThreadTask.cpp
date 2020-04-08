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
#include "Elpida/TaskFactory.hpp"
#include "Elpida/Topology/ProcessorNode.hpp"

namespace Elpida
{

	MultiThreadTask::MultiThreadTask(const std::string& name, const TaskFactory& taskFactory)
		: Task(name), _taskFactory(taskFactory), _threadsShouldWake(false)
	{

	}

	MultiThreadTask::~MultiThreadTask()
	{
		destroyTasks();
	}

	void MultiThreadTask::createTasks()
	{
		for (const auto& processor : _affinity.getProcessorNodes())
		{
			auto task = _taskFactory.create(*processor);
			_tasksToBeExecuted
				.push_back(TaskThread(*task, _wakeNotifier, _mutex, _threadsShouldWake, processor->getOsIndex()));
			_createdTasks.push_back(task);
		}
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

	void MultiThreadTask::prepare()
	{
		_threadsShouldWake = false;
		createTasks();
		for (auto& taskThread : _tasksToBeExecuted)
		{
			taskThread.getTask().prepare();
			taskThread.start();
		}
	}

	void MultiThreadTask::finalize()
	{
		for (auto& taskThread : _tasksToBeExecuted)
		{
			taskThread.getTask().finalize();
		}
		destroyTasks();
	}

	void MultiThreadTask::run()
	{
		std::unique_lock<std::mutex> lock(_mutex);
		_threadsShouldWake = true;
		lock.unlock();
		_wakeNotifier.notify_all();
		for (auto& task : _tasksToBeExecuted)
		{
			task.join();
		}
	}

} /* namespace Elpida */
