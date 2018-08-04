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

#include "Elpida/Task.hpp"
#include "Elpida/TaskThread.hpp"
#include "Elpida/Types/Array.hpp"
#include "Elpida/Types/String.hpp"

namespace Elpida
{

	class MultiThreadTask: public Task
	{
		public:

			inline void run()
			{
				for (auto& task : _tasksToBeExecuted)
				{
					task.start();
				}
				for (auto& task : _tasksToBeExecuted)
				{
					task.join();
				}
			}

			void addTask(Task& task);

			MultiThreadTask(const String& name, bool strictAffinity = false);
			virtual ~MultiThreadTask();

			MultiThreadTask(MultiThreadTask&&) = default;
			MultiThreadTask(const MultiThreadTask&) = default;
			MultiThreadTask& operator=(MultiThreadTask&&) = default;
			MultiThreadTask& operator=(const MultiThreadTask&) = default;
		private:
			Array<TaskThread> _tasksToBeExecuted;
			bool _strictAffinity;
	};

} /* namespace Elpida */

#endif /* ELPIDA_MULTITHREADTASK_HPP_ */
