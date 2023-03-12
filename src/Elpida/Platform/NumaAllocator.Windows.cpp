//
// Created by klapeto on 12/3/2023.
//

#include "Elpida/Core/Config.hpp"

#if defined(ELPIDA_WINDOWS)

#include "Elpida/Platform/NumaAllocator.hpp"

#include "Elpida/Platform/OsUtilities.hpp"
#include "Elpida/Core/ElpidaException.hpp"
#include "Elpida/Core/Topology/ProcessingUnitNode.hpp"

#include <windows.h>
#include <strsafe.h>

namespace Elpida
{
	void* NumaAllocator::Allocate(const ProcessingUnitNode& targetProcessingUnit, Size size) const
	{
		auto numaNodeId = targetProcessingUnit.GetNumaNode().GetOsIndex().value();
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

	unsigned int OsUtilities::GetNumaNodeIdForProcessor(unsigned int processorId)
	{
		PROCESSOR_NUMBER ProcessorNumber;

		if (!NT_SUCCESS(KeGetProcessorNumberFromIndex((ULONG)processorId, &ProcessorNumber))
		{
			throw ElpidaException("Failed to get the numa node id of processor '",processorId,"': ", OsUtilities::GetLastErrorString());
		}

		USHORT NodeNumber;

		GetNumaProcessorNodeEx(ProcessorNumber, &NodeNumber);
		return NodeNumber;
	}

} // Elpida

#endif // defined(ELPIDA_WINDOWS)