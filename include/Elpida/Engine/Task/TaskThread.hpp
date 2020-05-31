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
 * TaskThread.hpp
 *
 *  Created on: 17 Μαρ 2018
 *      Author: klapeto
 */

#ifndef ELPIDA_ENGINE_TASK_TASKTHREAD_HPP_
#define ELPIDA_ENGINE_TASK_TASKTHREAD_HPP_

#include <thread>
#include <condition_variable>
#include <vector>

namespace Elpida
{
	class Task;

	class TaskThread final
	{
	public:
		void start();
		void join();

		Task& getTaskToRun() const
		{
			return *_taskToRun;
		}

		static void setCurrentThreadAffinity(unsigned int cpuId);

		TaskThread(Task& taskToRun,
			std::condition_variable& waitNotifier,
			std::mutex& mutex,
			const bool& shouldWake,
			unsigned int affinity);
		virtual ~TaskThread();

		TaskThread(TaskThread&& other) noexcept;
	private:
		std::thread _runnerThread;
		Task* _taskToRun;
		std::condition_variable& _waitNotifier;
		std::mutex& _mutex;
		const bool& _shouldWake;
		unsigned int _affinity;

		void runTask();
	};

} /* namespace Elpida */

#endif /* ELPIDA_ENGINE_TASK_TASKTHREAD_HPP_ */
