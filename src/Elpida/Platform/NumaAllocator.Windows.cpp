//
// Created by klapeto on 12/3/2023.
//

#include "Elpida/Core/Config.hpp"

#if defined(ELPIDA_WINDOWS)

#include "Elpida/Platform/NumaAllocatorhpp"
#include "Elpida/Platform/OsUtilities.hpp"
#include "Elpida/Core/ElpidaException.hpp"

#include <windows.h>
#include <strsafe.h>

namespace Elpida
{
	void* NumaAllocator::Allocate(const ProcessingUnitNode& targetProcessingUnit, Size size) const
	{
		auto numaNodeId = numa_node_of_cpu(targetProcessingUnit.GetOsIndex().value());
		void* ptr = (void*)VirtualAllocExNuma(
			GetCurrentProcess(),
			NULL,
			size,
			MEM_RESERVE | MEM_COMMIT,
			PAGE_READWRITE,
			(UCHAR)numaNodeId
		);

		if (ptr == nullptr)
		{
			throw ElpidaException("Failed to allocate NUMA memory of size: ", size, " on numa node: ", numaNodeId, " (NUMA available: ", std::to_string(numaAvailable), "): ", OsUtilities::GetLastErrorString());
		}

		return ptr;
	}

	void NumaAllocator::Deallocate(void* ptr, Size size) const
	{
		VirtualFree(data, 0, MEM_RELEASE);
	}

} // Elpida

#endif // defined(ELPIDA_WINDOWS)