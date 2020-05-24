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
 * NumaMemory.cpp
 *
 *  Created on: 9 Ιουν 2019
 *      Author: klapeto
 */

#include "Elpida/Utilities/NumaMemory.hpp"
#include "Elpida/Config.hpp"

#ifdef ELPIDA_LINUX
#include <numa.h>
#include "Elpida/ElpidaException.hpp"
#else
#include <windows.h>
#endif

namespace Elpida
{
	NumaMemory::NumaMemory(std::size_t size, int node)
		: Memory(size), _node(node)
	{
	}

	NumaMemory::~NumaMemory()
	{
		deallocate(); // TODO: this is not needed, but be safe
	}

	void NumaMemory::allocateImpl()
	{
#ifdef ELPIDA_LINUX
		if (numa_available() < 0)
		{
			throw ElpidaException(FUNCTION_NAME, "Numa API is not available!");
		}
		_pointer = (pData)numa_alloc_onnode(_size, _node);
#else
		_pointer = (pData)VirtualAllocExNuma(
			GetCurrentProcess(),
			NULL,
			_size,
			MEM_RESERVE | MEM_COMMIT,
			PAGE_READWRITE,
			(UCHAR)_node
		);
#endif
		memset(_pointer, 0, _size);
	}

	void NumaMemory::deallocateImpl()
	{
#ifdef ELPIDA_LINUX
		numa_free(_pointer, _size);
#else
		VirtualFree(_pointer, 0, MEM_RELEASE);
#endif
	}

} /* namespace Elpida */

