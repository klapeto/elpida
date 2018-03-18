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
 * SingleThreadTask.hpp
 *
 *  Created on: 17 Μαρ 2018
 *      Author: klapeto
 */

#ifndef SRC_SINGLETHREADTASK_HPP_
#define SRC_SINGLETHREADTASK_HPP_

#include "Task.hpp"

namespace Elpida
{

	class SingleThreadTask: public Task
	{
		public:

			inline void run()
			{
				_taskToBeExecuted.run();
			}

			SingleThreadTask(Task& taskToBeExecuted) :
					Task(taskToBeExecuted.getName() + " (Single Threaded)"), _taskToBeExecuted(taskToBeExecuted)
			{

			}
			virtual ~SingleThreadTask()
			{

			}

			SingleThreadTask(SingleThreadTask&&) = default;
			SingleThreadTask(const SingleThreadTask&) = default;
			SingleThreadTask& operator=(SingleThreadTask&&) = default;
			SingleThreadTask& operator=(const SingleThreadTask&) = default;
		private:
			Task& _taskToBeExecuted;
	};

} /* namespace Elpida */

#endif /* SRC_SINGLETHREADTASK_HPP_ */
