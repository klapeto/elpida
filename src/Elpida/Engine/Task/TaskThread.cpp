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
 * TaskThread.cpp
 *
 *  Created on: 17 Μαρ 2018
 *      Author: klapeto
 */

#include "Elpida/Engine/Task/TaskThread.hpp"

#include <iostream>

#include "Elpida/Config.hpp"
#include "Elpida/Engine/Task/Task.hpp"

#ifdef ELPIDA_LINUX
#include <sched.h>
#else
#include <Windows.h>
#endif

namespace Elpida
{

	TaskThread::TaskThread(std::vector<Task*>&& tasksToRun,
		std::condition_variable& waitNotifier,
		std::mutex& mutex,
		const bool& shouldWake,
		unsigned int affinity)
		: _tasksToRun(std::move(tasksToRun)), _waitNotifier(waitNotifier), _mutex(mutex), _shouldWake(shouldWake),
		  _affinity(affinity)
	{
	}

	TaskThread::~TaskThread()
	{
		if (_runnerThread.joinable())
		{
			_runnerThread.join();
		}
		for (auto task: _tasksToRun)
		{
			delete task;
		}
	}

	void TaskThread::start()
	{
		_runnerThread = std::thread(&TaskThread::runTask, this);
	}

	void TaskThread::join()
	{
		if (_runnerThread.joinable())
		{
			_runnerThread.join();
		}
	}

	void TaskThread::runTask()
	{
		setCurrentThreadAffinity(_affinity);
		std::unique_lock<std::mutex> lock(_mutex);
		while (!_shouldWake)
		{
			_waitNotifier.wait(lock);
		}
		lock.unlock();
		for (auto task: _tasksToRun)
		{
			task->run();
		}
	}

	void TaskThread::setCurrentThreadAffinity(unsigned int cpuId)
	{
#ifdef ELPIDA_LINUX
		cpu_set_t mask;
		CPU_ZERO(&mask);
		CPU_SET(cpuId, &mask);
		if (sched_setaffinity(0, sizeof(cpu_set_t), &mask))
		{
			std::cout << "Warning! Failed to set affinity to " << cpuId << " on a thread!" << std::endl;
		}
#else
		SetThreadAffinityMask(GetCurrentThread(), 1 << (int)cpuId);
#endif
	}

} /* namespace Elpida */
