/*
 * MemoryReadWithAllocationTaskFactory.cpp
 *
 *  Created on: 30 Νοε 2019
 *      Author: klapeto
 */

#include "TaskBatches/Memory/Read/MemoryReadWithAllocationTaskFactory.hpp"
#include "TaskBatches/Memory/Read/MemoryReadWithAllocation.hpp"
#include "Elpida/Topology/ProcessorNode.hpp"

namespace Elpida
{

	Task* Elpida::MemoryReadWithAllocationTaskFactory::create(const ProcessorNode& processorAffinity) const
	{
		auto task = new MemoryReadWithAllocation(_sizePerTask, processorAffinity.getOsIndex());
		_createdTasks.push_back(task);
		return task;
	}

} /* namespace Elpida */

