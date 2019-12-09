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
 * NumaAllocatePerThread.cpp
 *
 *  Created on: 9 Ιουν 2019
 *      Author: klapeto
 */

#include "TaskBatches/General/NumaMemory.hpp"
#include "TaskBatches/General/NumaAllocatePerThread.hpp"
#include "TaskBatches/General/AlignedMemory.hpp"
#include "Elpida/Config.hpp"
#include "Elpida/Exceptions/ElpidaException.hpp"

#ifdef ELPIDA_LINUX
#include <numa.h>
#else
#include <windows.h>
#endif

namespace Elpida
{
	void NumaAllocatePerThread::prepare()
	{
#ifdef ELPIDA_LINUX
		auto threads = numa_num_configured_cpus();
		for (auto i = 0; i < threads; ++i)
		{
			_allocatedMemoryRegions.emplace(i, new NumaMemory(_memorySizePerThread, numa_node_of_cpu(i)));
		}
#else

		SYSTEM_INFO SystemInfo;
		GetSystemInfo(&SystemInfo);
		auto threads = SystemInfo.dwNumberOfProcessors;

		ULONG HighestNodeNumber;

		if (!GetNumaHighestNodeNumber(&HighestNodeNumber))
		{
			throw ElpidaException("Could not get Highest numa node. Err code: " + std::to_string(GetLastError()));
		}

		if (HighestNodeNumber == 0)
		{
			for (auto i = 0u; i < threads; ++i)
			{
				_allocatedMemoryRegions.emplace(i, new AlignedMemory(_memorySizePerThread, sizeof(void *) * 16));
			}
		}
		else
		{
			UCHAR NodeNumber;

			for (auto i = 0u; i < threads; ++i)
			{
				GetNumaProcessorNode(i, &NodeNumber);
				_allocatedMemoryRegions.emplace(i, new NumaMemory(_memorySizePerThread, NodeNumber));
			}
		}
#endif
	}

	void NumaAllocatePerThread::run()
	{
		for (auto& mem : _allocatedMemoryRegions)
		{
			mem.second->allocate();
		}
	}

	void NumaAllocatePerThread::calculateResults(const TaskMetrics& metrics)
	{
		_result.setOriginalValue(_memorySizePerThread);
		_result.setMultiplier(_allocatedMemoryRegions.size());
	}

	NumaAllocatePerThread::NumaAllocatePerThread(std::size_t memorySizePerThread)
			: Task("Numa Allocation Per Thread"), _memorySizePerThread(memorySizePerThread)
	{
	}

	NumaAllocatePerThread::~NumaAllocatePerThread()
	{
		for (auto& mem : _allocatedMemoryRegions)
		{
			delete mem.second;
		}
		_allocatedMemoryRegions.clear();
	}

} /* namespace Elpida */

