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
	class TaskFactory;

	class MultiThreadTask : public Task
	{
	public:
		void prepare() override;
		void finalize() override;
		void run() override;

		MultiThreadTask(const std::string& name, const TaskFactory& taskFactory);
		~MultiThreadTask() override;
	private:
		std::vector<Task*> _createdTasks;
		std::vector<TaskThread> _tasksToBeExecuted;
		std::mutex _mutex;
		std::condition_variable _wakeNotifier;
		const TaskFactory& _taskFactory;
		bool _threadsShouldWake;

		void destroyTasks();
		void createTasks();
	};

} /* namespace Elpida */

#endif /* ELPIDA_MULTITHREADTASK_HPP_ */
