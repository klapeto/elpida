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

#ifndef TASKBATCHES_MEMORY_MULTITHREADMEMORYCHUNKSREAD_HPP_
#define TASKBATCHES_MEMORY_MULTITHREADMEMORYCHUNKSREAD_HPP_

#include <Elpida/MultiThreadTask.hpp>
#include <Elpida/Types/Array.hpp>
#include "TaskBatches/General/MemoryChunk.hpp"
#include <Elpida/CpuInfo.hpp>
#include <Elpida/Types/Primitives.hpp>
#include "TaskBatches/Memory/MemoryReadCached.hpp"

namespace Elpida
{
	class MultiThreadMemoryChunksRead: public MultiThreadTask
	{
		public:

			void calculateResults() override
			{
				addResult(_totalBandwidth);
			}

			MultiThreadMemoryChunksRead(const Memory& memory, Size numberOfThreads)
					:
					  MultiThreadTask("Read " + ValueUtilities::getValueScaleString(memory.getSize()) + "B@ 8 Bytes/Read", true),
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
				_chunks = MemoryChunk::breakToChunks(_memory, _numberOfThreads);
				auto itterations = 0ul;
				for (const auto& chunk : _chunks)
				{
					auto mem = new MemoryReadCached(chunk, std::chrono::milliseconds(2000));
					if (itterations == 0ul)
					{
						itterations = mem->getItterations();
					}
					addTask(mem);
				}
				_totalBandwidth.setOriginalValue(_memory.getSize() * itterations);
			}
		private:
			TaskRunResult _totalBandwidth;
			Array<MemoryChunk> _chunks;
			Size _numberOfThreads;
			const Memory& _memory;
	};

} /* namespace Elpida */

#endif /* TASKBATCHES_MEMORY_MULTITHREADMEMORYCHUNKSREAD_HPP_ */
