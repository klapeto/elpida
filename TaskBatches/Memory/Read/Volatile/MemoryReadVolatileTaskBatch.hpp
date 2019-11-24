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
 * MemoryReadVolatileTaskBatch.hpp
 *
 *  Created on: 16 Μαΐ 2019
 *      Author: klapeto
 */

#ifndef TASKBATCHES_MEMORY_READ_VOLATILE_MEMORYREADVOLATILETASKBATCH_HPP_
#define TASKBATCHES_MEMORY_READ_VOLATILE_MEMORYREADVOLATILETASKBATCH_HPP_

#include <cstddef>

#include "TaskBatches/Memory/Read/Cached/MemoryReadCachedTaskBatch.hpp"

namespace Elpida
{

	class MemoryReadVolatileTaskBatch final: public MemoryReadCachedTaskBatch
	{
		public:
			MemoryReadVolatileTaskBatch()
					: MemoryReadCachedTaskBatch("Memory Read (Single Thread/Volatile)")
			{

			}

			~MemoryReadVolatileTaskBatch()
			{
			}
		protected:
			void addMemoryReadTask(std::size_t size) const override;
	};

} /* namespace Elpida */

#endif /* TASKBATCHES_MEMORY_READ_VOLATILE_MEMORYREADVOLATILETASKBATCH_HPP_ */
