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

#ifndef ELPIDA_TASKBATCH_HPP_
#define ELPIDA_TASKBATCH_HPP_

#include "Elpida/Types/Array.hpp"
#include "Elpida/Types/String.hpp"

namespace Elpida
{

	class Task;
	class TaskBatchConfiguration;

	class TaskBatch
	{
		public:

			typedef TaskBatch* (*TaskBatchGenerator)();

			const Array<Task*>& getTasks() const
			{
				return _tasks;
			}

			const String& getName() const
			{
				return _name;
			}

			virtual void reconfigure(const String& inputData)
			{

			}

			TaskBatch(const String& name)
					: _name(name)
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
			String _name;
		protected:
			Array<Task*> _tasks;
			void destroyTasks();
	};

} /* namespace Elpida */

#endif /* ELPIDA_TASKBATCH_HPP_ */
