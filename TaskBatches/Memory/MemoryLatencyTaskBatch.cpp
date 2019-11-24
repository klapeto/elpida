/*
 * MemoryLatencyTaskBatch.cpp
 *
 *  Created on: 19 Μαΐ 2019
 *      Author: klapeto
 */

#include "TaskBatches/Memory/MemoryLatencyTaskBatch.hpp"

#include "TaskBatches/General/AllocateMemory.hpp"
#include "TaskBatches/Memory/MemoryLatency.hpp"
#include "TaskBatches/Memory/WorkingSetSizes.hpp"
#include <Elpida/Topology/SystemTopology.hpp>

namespace Elpida
{

	void MemoryLatencyTaskBatch::addMemoryLatencyTask(Size size) const
	{
		auto affinity = TaskAffinity { 1 };

		auto memory = new AllocateMemory(size, true, SystemTopology::getNumaNodeOfProcessor(1));
		memory->setToBeMeasured(false);
		memory->setAffinity(affinity);

		auto latency = new MemoryLatency(memory->getMemory());
		latency->setAffinity(affinity);

		addTask(memory);
		addTask(latency);
	}

	void MemoryLatencyTaskBatch::createTasks() const
	{
		const auto sizes = {
//				WorkingSetSizes::B_4,
//				WorkingSetSizes::B_8,
//				WorkingSetSizes::B_16,
//				WorkingSetSizes::B_32,
//				WorkingSetSizes::B_64,
//				WorkingSetSizes::B_128,
				WorkingSetSizes::B_256,
				WorkingSetSizes::B_512,
				WorkingSetSizes::KiB_1,
				WorkingSetSizes::KiB_2,
				WorkingSetSizes::KiB_4,
				WorkingSetSizes::KiB_8,
				WorkingSetSizes::KiB_16,
				WorkingSetSizes::KiB_32,
				WorkingSetSizes::KiB_64,
				WorkingSetSizes::KiB_128,
				WorkingSetSizes::KiB_256,
				WorkingSetSizes::KiB_512,
				WorkingSetSizes::MiB_1,
				WorkingSetSizes::MiB_2,
				WorkingSetSizes::MiB_4,
				WorkingSetSizes::MiB_8,
				WorkingSetSizes::MiB_16,
				WorkingSetSizes::MiB_32,
				WorkingSetSizes::MiB_64,
				WorkingSetSizes::MiB_128,
				WorkingSetSizes::MiB_256,
				WorkingSetSizes::MiB_512,
		};

		for (auto size : sizes)
		{
			addMemoryLatencyTask(size);
		}
	}

} /* namespace Elpida */

