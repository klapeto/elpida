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
 * MultiThreadTask.hpp
 *
 *  Created on: 17 Μαρ 2018
 *      Author: klapeto
 */

#ifndef SRC_MULTITHREADTASK_HPP_
#define SRC_MULTITHREADTASK_HPP_

#include "Task.hpp"
#include "TaskThread.hpp"
#include <mutex>
#include <condition_variable>
#include <vector>

namespace Elpida
{

	class MultiThreadTask: public Task
	{
		public:

			void run()
			{
				std::unique_lock<std::mutex> lock(_mutex);
				for (auto& task : _tasksToBeExecuted)
				{
					task.getReadyToStart();
				}
				lock.unlock();
				for (auto& task : _tasksToBeExecuted)
				{
					task.join();
				}
			}

			MultiThreadTask(const std::vector<Task*>& tasks, bool strictAffinity);
			virtual ~MultiThreadTask();

			MultiThreadTask(MultiThreadTask&&) = default;
			MultiThreadTask(const MultiThreadTask&) = default;
			MultiThreadTask& operator=(MultiThreadTask&&) = default;
			MultiThreadTask& operator=(const MultiThreadTask&) = default;
		private:
			std::mutex _mutex;
			std::vector<TaskThread> _tasksToBeExecuted;
			bool _strictAffinity;
	};

} /* namespace Elpida */

#endif /* SRC_MULTITHREADTASK_HPP_ */
