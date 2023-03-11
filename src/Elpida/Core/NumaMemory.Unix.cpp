//
// Created by klapeto on 27/2/2023.
//

#include "Elpida/Core/Config.hpp"

#if defined(ELPIDA_UNIX)

#include "Elpida/Core/NumaMemory.hpp"

#include "Elpida/Core/ElpidaException.hpp"

#include <numa.h>

namespace Elpida
{

	void* NumaAllocator::Allocate(int numaNodeId, std::size_t size)
	{
		bool numaAvailable = false;
		void* ptr;
		if (numa_available() < 0)
		{
			ptr = malloc(size);
		}
		else
		{
			numa_set_strict(1);
			ptr = (void*)numa_alloc_onnode(size, numaNodeId);
			numaAvailable = true;
		}

		if (ptr == nullptr)
		{
			throw ElpidaException("Failed to allocate NUMA memory of size: ", size, " on numa node: ", numaNodeId, " (NUMA available: ", std::to_string(numaAvailable), ")");
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

	int NumaAllocator::GetProcessorNumaNode(int processorId)
	{
		return numa_node_of_cpu(processorId);
	}
}

#endif