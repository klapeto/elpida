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

#include "TaskBatches/General/MemoryChunk.hpp"
#include <Elpida/Exceptions/ElpidaException.hpp>

namespace Elpida
{

	Array<MemoryChunk> MemoryChunk::breakToChunks(const Memory& memory, Size chunks)
	{
		auto memorySize = memory.getSize();

		if (chunks > memorySize) throw ElpidaException("MemoryChunk::breakToChunks",
		                                               "Attempted to break to chunks that are greater than the memory size");
		if (memory.getPointer() == nullptr) throw ElpidaException("MemoryChunk::breakToChunks",
		                                                          "Attempted to break to chunks memory that is not allocated");

		auto returnArray = Array<MemoryChunk>();
		auto ptr = memory.getPointer();
		auto chunkSize = memorySize / chunks;
		for (Size i = 0; i < chunks; ++i)
		{
			auto thisChunkSize = chunkSize;
			auto curPtr = i * chunkSize;
			if (curPtr + thisChunkSize > memorySize)
			{
				thisChunkSize = memorySize - (i * chunkSize);
			}
			returnArray.push_back(MemoryChunk((void*) ((Size) ptr + (i * chunkSize)), thisChunkSize));
		}
		return returnArray;
	}

} /* namespace Elpida */

