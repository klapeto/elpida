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
 * MultiThreadMemoryRead.hpp
 *
 *  Created on: 7 Ιαν 2019
 *      Author: klapeto
 */

#ifndef TASKBATCHES_MEMORY_READ_MULTITHREADMEMORYREAD_HPP_
#define TASKBATCHES_MEMORY_READ_MULTITHREADMEMORYREAD_HPP_

#include <Elpida/MultiThreadTask.hpp>
#include <Elpida/TaskRunResult.hpp>

namespace Elpida
{
	class Memory;

	class MultiThreadMemoryRead: MultiThreadTask
	{
		private:
			struct MemoryBlock
			{
					const Memory* memory;
					int affinity;
			};
		public:

			void calculateResults(const TaskMetrics& metrics) override
			{
				addResult(_totalBandwidth);
			}

			void addMemoryBlock(const Memory& memory, int affinity = -1)
			{
				_blocks.push_back( { &memory, affinity });

			}

			MultiThreadMemoryRead()
					: MultiThreadTask("Read 8 Bytes/Read")
			{

			}
			~MultiThreadMemoryRead()
			{

			}
		private:
			Array<MemoryBlock> _blocks;
			TaskRunResult _totalBandwidth;

	};

} /* namespace Elpida */

#endif /* TASKBATCHES_MEMORY_READ_MULTITHREADMEMORYREAD_HPP_ */
