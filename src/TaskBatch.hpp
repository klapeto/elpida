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
 * TaskBatch.hpp
 *
 *  Created on: 13 Μαρ 2018
 *      Author: klapeto
 */

#ifndef SRC_TASKBATCH_HPP_
#define SRC_TASKBATCH_HPP_

#include <string>
#include <vector>

namespace Elpida
{

	class Task;
	class TaskBatchConfiguration;

	class TaskBatch
	{
		public:

			const std::vector<Task*>& getTasks() const
			{
				return _tasks;
			}

			const std::string& getName() const
			{
				return _name;
			}

			virtual void reconfigure()
			{

			}

			TaskBatch(const std::string& name) :
					_name(name)
			{

			}

			virtual ~TaskBatch()
			{

			}

			TaskBatch(TaskBatch&&) = default;
			TaskBatch(const TaskBatch&) = default;
			TaskBatch& operator=(TaskBatch&&) = default;
			TaskBatch& operator=(const TaskBatch&) = default;

		private:
			std::string _name;
		protected:
			std::vector<Task*> _tasks;
			void destroyTasks();
	};

} /* namespace Elpida */

#endif /* SRC_TASKBATCH_HPP_ */
