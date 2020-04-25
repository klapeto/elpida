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
 * MemoryChunk.cpp
 *
 *  Created on: 21 Οκτ 2018
 *      Author: klapeto
 */

#include "Elpida/Utilities/MemoryChunk.hpp"

#include "Elpida/ElpidaException.hpp"

namespace Elpida
{

	MemoryChunk::MemoryChunk(void* pointer, std::size_t size)
		: Memory(size), _ptr(pointer)
	{
		_pointer = _ptr;
	}

	MemoryChunk::MemoryChunk(MemoryChunk&& other) noexcept
		: Memory(std::move(other))
	{
		this->_ptr = other._ptr;
		other._ptr = nullptr;
	}

	MemoryChunk::~MemoryChunk()
	{
		_pointer = nullptr;
	}

	std::vector<MemoryChunk> MemoryChunk::breakToChunks(const Memory& memory, std::size_t chunks)
	{
		auto memorySize = memory.getSize();

		if (chunks > memorySize)
			throw ElpidaException("MemoryChunk::breakToChunks",
				"Attempted to break to chunks that are greater than the memory size");
		if (memory.getPointer() == nullptr)
			throw ElpidaException("MemoryChunk::breakToChunks",
				"Attempted to break to chunks memory that is not allocated");

		auto returnArray = std::vector<MemoryChunk>();
		auto ptr = memory.getPointer();
		auto chunkSize = memorySize / chunks;
		for (std::size_t i = 0; i < chunks; ++i)
		{
			auto thisChunkSize = chunkSize;
			auto curPtr = i * chunkSize;
			if (curPtr + thisChunkSize > memorySize)
			{
				thisChunkSize = memorySize - (i * chunkSize);
			}
			returnArray.emplace_back((void*)((std::size_t)ptr + (i * chunkSize)), thisChunkSize);
		}
		return returnArray;
	}

} /* namespace Elpida */

