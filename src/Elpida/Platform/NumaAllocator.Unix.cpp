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
// Created by klapeto on 11/3/2023.
//

#include "Elpida/Core/Config.hpp"
#include <cstdlib>

#if defined(ELPIDA_UNIX)

#include "Elpida/Platform/NumaAllocator.hpp"

#include "Elpida/Core/ElpidaException.hpp"
#include "Elpida/Core/Topology/ProcessingUnitNode.hpp"

#include <numa.h>
#include <cstring>

namespace Elpida
{
	void* NumaAllocator::Allocate(Size size)
	{
		auto a = Timer::now();
		auto numaNodeId = _targetProcessingUnit.GetNumaNode().GetOsIndex().value();
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
			throw ElpidaException("Failed to allocate NUMA memory of size: ",
				size,
				" on numa node: ",
				numaNodeId,
				" (NUMA available: ",
				std::to_string(numaAvailable),
				")");
		}

		std::memset(ptr, 0 , size);	// commit immediately

		_totalAllocations++;

		auto b = Timer::now();
		_totalTime += b - a;
		return ptr;
	}

	void NumaAllocator::Deallocate(void* ptr, Size size) noexcept
	{
		auto a = Timer::now();
		if (numa_available() < 0)
		{
			free(ptr);
		}
		else
		{
			numa_free(ptr, size);
		}
		auto b = Timer::now();
		_totalTime += b - a;
	}

	void* NumaAllocator::Reallocate(void* ptr, Size oldSize, Size newSize)
	{
		if (ptr == nullptr) return Allocate(newSize);

		auto a = Timer::now();
		void* newPtr = nullptr;
		if (numa_available() < 0)
		{
			newPtr = realloc(ptr, newSize);
		}
		else
		{
			newPtr =  numa_realloc(ptr, oldSize, newSize);
		}

		if (newSize > oldSize)
		{
			std::memset(((char*) ptr) + oldSize, 0 , newSize - oldSize);	// commit immediately
		}

		auto b = Timer::now();
		_totalTime += b - a;
		return newPtr;
	}

	NumaAllocator::NumaAllocator(const ProcessingUnitNode& targetProcessingUnit)
		: _targetProcessingUnit(targetProcessingUnit)
	{
	}
} // Elpida

#endif // defined(ELPIDA_UNIX)