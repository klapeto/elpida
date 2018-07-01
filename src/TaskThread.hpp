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
 * TaskThread.hpp
 *
 *  Created on: 17 Μαρ 2018
 *      Author: klapeto
 */

#ifndef SRC_TASKTHREAD_HPP_
#define SRC_TASKTHREAD_HPP_

#include <thread>

namespace Elpida
{
	class Task;
	class TaskThread
	{
		public:

			int getAffinity() const
			{
				return _affinity;
			}

			void setAffinity(int affinity)
			{
				_affinity = affinity;
			}

			void start();
			void join();

			static void setCurrentThreadAffinity(int cpuId);

			TaskThread(Task& task, int affinity = -1);
			virtual ~TaskThread();

			TaskThread(TaskThread&&) = default;
			TaskThread(const TaskThread&) = delete;
			TaskThread& operator=(TaskThread&&) = default;
			TaskThread& operator=(const TaskThread&) = delete;

		private:
			std::thread _runnerThread;
			Task& _task;
			int _affinity;

			void runTask();
	};

} /* namespace Elpida */

#endif /* SRC_TASKTHREAD_HPP_ */
