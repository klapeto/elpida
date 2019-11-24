/*
 * NumaAllocatePerThread.cpp
 *
 *  Created on: 9 Ιουν 2019
 *      Author: klapeto
 */

#include "TaskBatches/General/NumaMemory.hpp"
#include "TaskBatches/General/NumaAllocatePerThread.hpp"
#include "TaskBatches/General/AlignedMemory.hpp"
#include <Elpida/Config.hpp>
#include <Elpida/Exceptions/ElpidaException.hpp>

#ifdef _elpida_linux
#include <numa.h>
#else
#include <windows.h>
#endif

namespace Elpida
{
	void NumaAllocatePerThread::prepare()
	{
#ifdef _elpida_linux
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

	NumaAllocatePerThread::NumaAllocatePerThread(Size memorySizePerThread)
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

