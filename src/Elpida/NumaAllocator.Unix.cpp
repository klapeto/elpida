//
// Created by klapeto on 27/2/2023.
//

#include "Elpida/Config.hpp"

#if defined(ELPIDA_UNIX)

#include "Elpida/NumaAllocator.hpp"
#include <numa.h>

namespace Elpida
{

	void* NumaAllocator::Allocate(long numaNodeId, std::size_t size)
	{
		if (numa_available() < 0)
		{
			return malloc(size);
		}

		numa_set_strict(1);
		auto ptr = (void*)numa_alloc_onnode(size, numaNodeId);

		if (ptr == nullptr)
		{
			//throw;
		}

		return ptr;
	}

	void NumaAllocator::Deallocate(void* ptr, std::size_t size)
	{
		if (numa_available() < 0)
		{
			free(ptr);
		}
		else
		{
			numa_free(ptr, size);
		}
	}
}

#endif