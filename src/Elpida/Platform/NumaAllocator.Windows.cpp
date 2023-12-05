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
#include <memoryapi.h>

namespace Elpida
{
	void* NumaAllocator::Allocate(const ProcessingUnitNode& targetProcessingUnit, Size size) const
	{
		auto a = Timer::now();
		auto numaNodeId = targetProcessingUnit.GetNumaNode().GetOsIndex().value();
#if _WIN32_WINNT >= 0x0600
		void* ptr = (void*)VirtualAllocExNuma(
			GetCurrentProcess(),
			NULL,
			size,
			MEM_RESERVE | MEM_COMMIT,
			PAGE_READWRITE,
			(UCHAR)numaNodeId
		);
#else
		// Very old windows, no numa support
		void* ptr = (void*)VirtualAllocEx(
			GetCurrentProcess(),
			NULL,
			size,
			MEM_RESERVE | MEM_COMMIT,
			PAGE_READWRITE
		);
#endif

		if (ptr == nullptr)
		{
			throw ElpidaException("Failed to allocate NUMA memory of size: ", size, " on numa node: ", numaNodeId, ": ", OsUtilities::GetLastErrorString());
		}

		auto b = Timer::now();
		_totalTime += b - a;

		return ptr;
	}

	void NumaAllocator::Deallocate(void* ptr, Size size) const
	{
		auto a = Timer::now();
		VirtualFree(ptr, 0, MEM_RELEASE);

		auto b = Timer::now();
		_totalTime += b - a;
	}

} // Elpida

#endif // defined(ELPIDA_WINDOWS)