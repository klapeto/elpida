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
 * Task.hpp
 *
 *  Created on: 17 Μαρ 2018
 *      Author: klapeto
 */

#ifndef SRC_TASK_HPP_
#define SRC_TASK_HPP_

#include <string>
#include "TaskMetrics.hpp"
#include "TaskThroughput.hpp"

namespace Elpida
{

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

			inline const std::string& getName() const
			{
				return _name;
			}

			virtual void run() = 0;
			virtual TaskThroughput translateToThroutput(const TaskMetrics& metrics) const = 0;

			Task(const std::string& name, bool toBeMeasured = true) :
					_name(name), _toBeMeasured(toBeMeasured)
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
			std::string _name;
			bool _toBeMeasured;
	};

} /* namespace Elpida */

#endif /* SRC_TASK_HPP_ */
