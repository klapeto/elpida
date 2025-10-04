/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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
#if _WIN32_WINNT >= 0x0600
#include <memoryapi.h>
#endif

namespace Elpida
{
	void* NumaAllocator::Allocate(Size size)
	{
		auto a = Timer::now();
		auto numaNodeId = _targetProcessingUnit.GetNumaNode().GetOsIndex().value();
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

	void* NumaAllocator::Reallocate(void* ptr, Size oldSize, Size newSize)
	{
		auto a = Timer::now();
		auto numaNodeId = _targetProcessingUnit.GetNumaNode().GetOsIndex().value();
#if _WIN32_WINNT >= 0x0600
		void* newPtr = (void*)VirtualAllocExNuma(
			GetCurrentProcess(),
			ptr,
			newSize,
			MEM_RESERVE | MEM_COMMIT,
			PAGE_READWRITE,
			(UCHAR)numaNodeId
		);
#else
		// Very old windows, no numa support
		void* newPtr = (void*)VirtualAllocEx(
			GetCurrentProcess(),
			ptr,
			newSize,
			MEM_RESERVE | MEM_COMMIT,
			PAGE_READWRITE
		);
#endif

		if (newPtr == nullptr)
		{
			throw ElpidaException("Failed to allocate NUMA memory of size: ", newSize, " on numa node: ", numaNodeId, ": ", OsUtilities::GetLastErrorString());
		}

		auto b = Timer::now();
		_totalTime += b - a;

		return newPtr;
	}

	void NumaAllocator::Deallocate(void* ptr, Size size) noexcept
	{
		auto a = Timer::now();
		VirtualFree(ptr, 0, MEM_RELEASE);

		auto b = Timer::now();
		_totalTime += b - a;
	}

	NumaAllocator::NumaAllocator(const ProcessingUnitNode& targetProcessingUnit)
		: _targetProcessingUnit(targetProcessingUnit)
	{
	}


} // Elpida

#endif // defined(ELPIDA_WINDOWS)