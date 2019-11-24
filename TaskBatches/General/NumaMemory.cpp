/*
 * NumaMemory.cpp
 *
 *  Created on: 9 Ιουν 2019
 *      Author: klapeto
 */

#include "TaskBatches/General/NumaMemory.hpp"
#include <Elpida/Config.hpp>
#ifdef _elpida_linux
#include <numa.h>
#else
#include <windows.h>
#endif

namespace Elpida
{

	void NumaMemory::allocateImpl()
	{
#ifdef _elpida_linux	
		_pointer = numa_alloc_onnode(_size, _node);
#else		
		_pointer =VirtualAllocExNuma(
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
#ifdef _elpida_linux
		numa_free(_pointer, _size);
#else
		VirtualFree(_pointer,0, MEM_RELEASE);
		#endif
	}

} /* namespace Elpida */

