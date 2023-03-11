//
// Created by klapeto on 11/3/2023.
//

#include "Elpida/Core/Config.hpp"

#if defined(ELPIDA_UNIX)

#include "Elpida/Platform/NumaAllocator.hpp"

#include "Elpida/Core/ElpidaException.hpp"
#include "Elpida/Core/Topology/ProcessingUnitNode.hpp"

#include <numa.h>

namespace Elpida
{
	void* NumaAllocator::Allocate(const ProcessingUnitNode& targetProcessingUnit, Size size) const
	{
		auto numaNodeId = targetProcessingUnit.GetNumaNode().value().get().GetOsIndex().value();
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

	void NumaAllocator::Deallocate(void* ptr, Size size) const
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
} // Elpida


#endif // defined(ELPIDA_UNIX)