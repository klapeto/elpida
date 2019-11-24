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
 * MemoryReadVolatile.hpp
 *
 *  Created on: 16 Μαΐ 2019
 *      Author: klapeto
 */

#ifndef TASKBATCHES_MEMORY_READ_VOLATILE_MEMORYREADVOLATILE_HPP_
#define TASKBATCHES_MEMORY_READ_VOLATILE_MEMORYREADVOLATILE_HPP_

#include "TaskBatches/Memory/Read/Cached/MemoryReadCached.hpp"

namespace Elpida
{

	template<typename T = int64_t>
	class MemoryReadVolatile final: public MemoryReadCached<T>
	{
		public:

			virtual void run() override
			{

			}

			MemoryReadVolatile(const Memory& memory)
					: MemoryReadCached<T>(memory)
			{

			}

			virtual ~MemoryReadVolatile()
			{

			}
	};

} /* namespace Elpida */

#endif /* TASKBATCHES_MEMORY_READ_VOLATILE_MEMORYREADVOLATILE_HPP_ */
