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
 * TaskThread.hpp
 *
 *  Created on: 17 Μαρ 2018
 *      Author: klapeto
 */

#ifndef SRC_TASKTHREAD_HPP_
#define SRC_TASKTHREAD_HPP_

#include <thread>
#include <mutex>

namespace Elpida
{
	class Task;
	class TaskThread
	{
		public:

			void getReadyToStart();
			void join();

			TaskThread(Task& task, std::mutex& mutex, int affinity = -1);
			virtual ~TaskThread();

			TaskThread(TaskThread&&) = default;
			TaskThread(const TaskThread&) = delete;
			TaskThread& operator=(TaskThread&&) = default;
			TaskThread& operator=(const TaskThread&) = delete;
		private:
			std::thread _runnerThread;
			Task& _task;
			std::mutex& _mutex;
			int _affinity;

			void runTask();
			static void setCurrentThreadAffinity(int cpuId);
	};

} /* namespace Elpida */

#endif /* SRC_TASKTHREAD_HPP_ */
