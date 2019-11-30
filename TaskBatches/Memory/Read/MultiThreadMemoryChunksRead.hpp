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
 * MultiThreadMemoryChunksRead.hpp
 *
 *  Created on: 20 Οκτ 2018
 *      Author: klapeto
 */

#ifndef TASKBATCHES_MEMORY_READ_MULTITHREADMEMORYCHUNKSREAD_HPP_
#define TASKBATCHES_MEMORY_READ_MULTITHREADMEMORYCHUNKSREAD_HPP_

#include <cstddef>
#include <unordered_map>
#include <utility>

#include "Elpida/MultiThreadTask.hpp"
#include "Elpida/TaskRunResult.hpp"
#include "TaskBatches/Memory/Read/MemoryReadTaskFactory.hpp"

namespace Elpida
{
	class TaskMetrics;

	template<typename T>
	class MultiThreadMemoryChunksRead: public MultiThreadTask
	{
		public:

			void calculateResults(const TaskMetrics& metrics) override
			{
				std::size_t totalItterations = 1;
				auto& tasks = _taskFactory.getCreatedTasks();
				if (tasks.size() > 0)
				{
					totalItterations = (*tasks.begin())->getIterations();
				}
				_totalBandwidth.setOriginalValue(_taskFactory.getSizePerTask() * tasks.size());
				_totalBandwidth.setMultiplier(totalItterations);
				addResult(_totalBandwidth);
				_taskFactory.resetCreatedTasks();
			}

			MultiThreadMemoryChunksRead(std::size_t sizePerThread)
					:
					  MultiThreadTask("Read @ 8 Bytes/Read", _taskFactory),
					  _taskFactory(sizePerThread),
					  _totalBandwidth("Total Read Bandwidth", "Bytes")
			{

			}
			virtual ~MultiThreadMemoryChunksRead()
			{

			}
		private:
			MemoryReadTaskFactory<T> _taskFactory;
			TaskRunResult _totalBandwidth;
	};

} /* namespace Elpida */

#endif /* TASKBATCHES_MEMORY_READ_MULTITHREADMEMORYCHUNKSREAD_HPP_ */
