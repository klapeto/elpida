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

#ifndef TASKBATCHES_MEMORY_READ_MEMORYREADWITHALLOCATION_HPP_
#define TASKBATCHES_MEMORY_READ_MEMORYREADWITHALLOCATION_HPP_

#include <vector>

#include "TaskBatches/Memory/Read/Cached/MemoryReadCached.hpp"
#include "TaskBatches/General/NumaMemory.hpp"
#include "Elpida/TaskRunResult.hpp"

namespace Elpida
{

	class MemoryReadWithAllocation: public MemoryReadCached<>
	{
		public:

			virtual void prepare() override
			{
				_memory.allocate();
			}

			virtual void finalize() override
			{
				_memory.deallocate();
			}

			MemoryReadWithAllocation(std::size_t size, unsigned int affinity)
					: MemoryReadCached(_memory), _memory(size, affinity)
			{
				_iterations = _iterationConstant / (double)size;
			}

			virtual ~MemoryReadWithAllocation()
			{

			}
		private:
			NumaMemory _memory;
	};

} /* namespace Elpida */

#endif /* TASKBATCHES_MEMORY_READ_MEMORYREADWITHALLOCATION_HPP_ */
