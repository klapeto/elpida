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
 * MemoryChunk.hpp
 *
 *  Created on: 21 Οκτ 2018
 *      Author: klapeto
 */

#ifndef TASKBATCHES_GENERAL_MEMORYCHUNK_HPP_
#define TASKBATCHES_GENERAL_MEMORYCHUNK_HPP_

#include "TaskBatches/General/Memory.hpp"
#include <Elpida/Types/Array.hpp>
#include <Elpida/Utilities/NonCopyable.hpp>

namespace Elpida
{

	class MemoryChunk final: public Memory
	{
		public:

			static Array<MemoryChunk> breakToChunks(const Memory& memory, Size chunks);

			void allocateImpl() override
			{
				_pointer = _ptr;
			}

			void deallocateImpl() override
			{
				_pointer = nullptr;
			}

			MemoryChunk(void* pointer, Size size)
					: Memory(size), _ptr(pointer)
			{
				_pointer = _ptr;
			}

			~MemoryChunk()
			{
				_pointer = nullptr;
			}

			MemoryChunk(MemoryChunk&& other)
					: Memory(std::move(other))
			{
				this->_ptr = other._ptr;
				other._ptr = nullptr;
			}
		private:
			void* _ptr;
	};

} /* namespace Elpida */

#endif /* TASKBATCHES_GENERAL_MEMORYCHUNK_HPP_ */
