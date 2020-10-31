/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2020  Ioannis Panagiotopoulos
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
#include "Elpida/Utilities/OsUtilities.hpp"

#ifdef ELPIDA_LINUX
#include <sched.h>
#else
#include <windows.h>
#endif

namespace Elpida
{

	TaskThread::TaskThread(Task& taskToRun,
		std::condition_variable& waitNotifier,
		std::mutex& mutex,
		const bool& shouldWake,
		unsigned int affinity)
		: _taskToRun(&taskToRun),
		  _waitNotifier(waitNotifier),
		  _mutex(mutex),
		  _shouldWake(shouldWake),
		  _affinity(affinity)
	{
	}

	TaskThread::TaskThread(TaskThread&& other) noexcept
		: _taskToRun(other._taskToRun),
		  _waitNotifier(other._waitNotifier),
		  _mutex(other._mutex),
		  _shouldWake(other._shouldWake),
		  _affinity(other._affinity)
	{
		other._taskToRun = nullptr;
	}

	TaskThread::~TaskThread()
	{
		if (_runnerThread.joinable())
		{
			_runnerThread.join();
		}
		delete _taskToRun;
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
		OsUtilities::setCurrentThreadAffinity(_affinity);
		std::unique_lock<std::mutex> lock(_mutex);
		while (!_shouldWake)
		{
			_waitNotifier.wait(lock);
		}
		lock.unlock();
		_taskToRun->execute();
	}

} /* namespace Elpida */
