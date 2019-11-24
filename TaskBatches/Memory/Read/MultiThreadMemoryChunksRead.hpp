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
#include "TaskBatches/Memory/Read/Volatile/MemoryReadVolatile.hpp"

namespace Elpida
{
	class TaskMetrics;

	class MultiThreadMemoryChunksRead: public MultiThreadTask
	{
		public:

			void calculateResults(const TaskMetrics& metrics) override
			{
				addResult(_totalBandwidth);
			}

			MultiThreadMemoryChunksRead(const std::unordered_map<int, Memory*>& memory, std::size_t numberOfThreads)
					:
					  MultiThreadTask("Read @ 8 Bytes/Read"),
					  _totalBandwidth("Total Read Bandwidth", "Bytes"),
					  _numberOfThreads(numberOfThreads),
					  _memory(memory)
			{

			}
			virtual ~MultiThreadMemoryChunksRead()
			{

			}

		protected:
			void createTasks()
			{
				auto iterations = 0ul;
				auto totalSize = 0ul;
				auto cpu = 0ul;
				for (const auto& chunk : _memory)
				{
					totalSize += chunk.second->getSize();
					auto mem = new MemoryReadVolatile<>(*chunk.second);
					if (iterations == 0ul)
					{
						iterations = mem->getIterations();
					}
					addTask(mem, chunk.first);
				}
				_totalBandwidth.setOriginalValue(totalSize * iterations);
			}
		private:
			TaskRunResult _totalBandwidth;
			std::size_t _numberOfThreads;
			const std::unordered_map<int, Memory*>& _memory;
	};

} /* namespace Elpida */

#endif /* TASKBATCHES_MEMORY_READ_MULTITHREADMEMORYCHUNKSREAD_HPP_ */
