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
 * Runner.hpp
 *
 *  Created on: 8 Μαρ 2018
 *      Author: klapeto
 */

#ifndef SRC_RUNNER_HPP_
#define SRC_RUNNER_HPP_

#include <string>
#include <vector>

#include "Task.hpp"

namespace Elpida
{
	class Task;

	class Runner
	{
		public:
			enum class ProcessPriority
			{
				Normal, High,
			};


			static void setProcessPriority(ProcessPriority priority);

			void executeTasks() const;

			Runner();
			~Runner();

			Runner(Runner&&) = default;
			Runner(const Runner&) = default;
			Runner& operator=(Runner&&) = default;
			Runner& operator=(const Runner&) = default;
		private:
			std::vector<Task*> _tasks;
	};

} /* namespace Elpida */

#endif /* SRC_RUNNER_HPP_ */
