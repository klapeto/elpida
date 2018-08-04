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
 * Task.hpp
 *
 *  Created on: 17 Μαρ 2018
 *      Author: klapeto
 */

#ifndef ELPIDA_TASK_HPP_
#define ELPIDA_TASK_HPP_

#include "Elpida/Types/String.hpp"
#include "Elpida/TaskThroughput.hpp"

namespace Elpida
{
	class TaskMetrics;

	class Task
	{
		public:

			bool isToBeMeasured() const
			{
				return _toBeMeasured;
			}

			void setToBeMeasured(bool toBeMeasured)
			{
				_toBeMeasured = toBeMeasured;
			}

			inline const String& getName() const
			{
				return _name;
			}

			virtual void run() = 0;
			virtual TaskThroughput translateToThroutput(const TaskMetrics& metrics) const = 0;

			virtual void prepare()
			{

			}

			Task(const String& name, bool toBeMeasured = true)
					: _name(name), _toBeMeasured(toBeMeasured)
			{

			}
			virtual ~Task()
			{

			}

			Task(Task&&) = default;
			Task(const Task&) = default;
			Task& operator=(Task&&) = default;
			Task& operator=(const Task&) = default;

		private:
			String _name;
			bool _toBeMeasured;
	};

} /* namespace Elpida */

#endif /* ELPIDA_TASK_HPP_ */
