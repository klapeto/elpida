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
 * MultiThreadTask.hpp
 *
 *  Created on: 17 Μαρ 2018
 *      Author: klapeto
 */

#ifndef ELPIDA_MULTITHREADTASK_HPP_
#define ELPIDA_MULTITHREADTASK_HPP_

#include <condition_variable>
#include <mutex>
#include <string>
#include <vector>

#include "Elpida/Task.hpp"
#include "Elpida/TaskThread.hpp"

namespace Elpida
{

	class MultiThreadTask: public Task
	{
		public:

			void prepare() override
			{
				_threadsShouldWake = false;
				createTasks();
				for (auto& taskThread : _tasksToBeExecuted)
				{
					taskThread.getTask().prepare();
					taskThread.start();
				}
			}

			void finalize() override
			{
				for (auto& taskThread : _tasksToBeExecuted)
				{
					taskThread.getTask().finalize();
				}
				destroyTasks();
			}

			inline void run() override
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

			MultiThreadTask(const std::string& name);
			virtual ~MultiThreadTask();

			MultiThreadTask(MultiThreadTask&&) = default;
			MultiThreadTask(const MultiThreadTask&) = default;
			MultiThreadTask& operator=(MultiThreadTask&&) = default;
			MultiThreadTask& operator=(const MultiThreadTask&) = default;

		protected:
			virtual void createTasks() = 0;
			void addTask(Task* task, int affinity = -1);
		private:
			std::vector<Task*> _createdTasks;
			std::vector<TaskThread> _tasksToBeExecuted;
			std::mutex _mutex;
			std::condition_variable _wakeNotifier;
			bool _threadsShouldWake;
			void destroyTasks();
	};

} /* namespace Elpida */

#endif /* ELPIDA_MULTITHREADTASK_HPP_ */
