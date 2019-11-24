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

#include <cstdint>
#include "TaskBatches/Memory/Read/Cached/MemoryReadCached.hpp"

namespace Elpida
{

	template<typename T = int64_t>
	class MemoryReadVolatile final: public MemoryReadCached<T>
	{
		public:

			virtual void run() override
			{
				volatile auto ptr = (T*) this->_memory.getPointer();
				register auto start = ptr;
				register auto end = (T *) ((T) start + this->_memory.getSize());
				register auto iterations = this->_iterations;
				register auto x = T();
				for (register auto i = 0ul; i < iterations; ++i)
				{
					ptr = start;
					while (ptr < end)
					{
						x = *ptr;
						x = *(ptr + 1);
						x = *(ptr + 2);
						x = *(ptr + 3);
						x = *(ptr + 4);
						x = *(ptr + 5);
						x = *(ptr + 6);
						x = *(ptr + 7);
						x = *(ptr + 8);
						x = *(ptr + 9);
						x = *(ptr + 10);
						x = *(ptr + 11);
						x = *(ptr + 12);
						x = *(ptr + 13);
						x = *(ptr + 14);
						x = *(ptr + 15);
						x = *(ptr + 16);
						x = *(ptr + 17);
						x = *(ptr + 18);
						x = *(ptr + 19);
						x = *(ptr + 20);
						x = *(ptr + 21);
						x = *(ptr + 22);
						x = *(ptr + 23);
						x = *(ptr + 24);
						x = *(ptr + 25);
						x = *(ptr + 26);
						x = *(ptr + 27);
						x = *(ptr + 28);
						x = *(ptr + 29);
						x = *(ptr + 30);
						x = *(ptr + 31);
						ptr += 32;
					}
				}
				auto dummy = x;
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
