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

#include "Elpida/TaskThread.hpp"

#include <iostream>

#include "Config.hpp"
#include "Elpida/Task.hpp"

#if _elpida_linux
#include <sched.h>
#elif _elpida_windows
#include <Windows.h>
#endif

namespace Elpida
{

	TaskThread::TaskThread(Task& task, std::condition_variable& waitNotifier, std::mutex& mutex, const bool& shouldWake, int affinity)
			: _task(task), _waitNotifier(waitNotifier), _mutex(mutex), _shouldWake(shouldWake), _affinity(affinity)
	{
	}

	TaskThread::~TaskThread()
	{
		if (_runnerThread.joinable())
		{
			_runnerThread.join();
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
		if (_affinity != -1)
		{
			setCurrentThreadAffinity(_affinity);
		}
		std::unique_lock<std::mutex> lock(_mutex);
		while (!_shouldWake)
		{
			_waitNotifier.wait(lock);
		}
		lock.unlock();
		_task.run();
	}

	void TaskThread::setCurrentThreadAffinity(int cpuId)
	{
#if _elpida_linux
		cpu_set_t mask;
		CPU_ZERO(&mask);
		CPU_SET(cpuId, &mask);
		if (sched_setaffinity(0, sizeof(cpu_set_t), &mask))
		{
			std::cout << "Warning! Failed to set affinity to " << cpuId << " on a thread!" << std::endl;
		}
#elif _elpida_windows
		SetThreadAffinityMask(GetCurrentThread(), 1 << (int)cpuId);
#endif
	}

} /* namespace Elpida */
