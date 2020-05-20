//
// Created by klapeto on 30/4/20.
//

#include "Benchmarks/Memory/Latency/MemoryReadLatencySpecification.hpp"
#include "Benchmarks/Memory/Latency/MemoryReadLatency.hpp"

namespace Elpida
{

	MemoryReadLatencySpecification::MemoryReadLatencySpecification()
		: TaskSpecification("Memory Read Latency",
		ResultSpecification("Access time", "s", ResultSpecification::Raw, ResultSpecification::Average))
	{
		withDescription("Reads continuously a memory region to determine memory read latency");
		withInputData(DataSpecification("Input Memory",
			"B",
			32 * sizeof(RegisterSize),
			"The allocated memory region to read"));
	}

	Task* MemoryReadLatencySpecification::createNewTask(const TaskConfiguration& configuration,
		const TaskAffinity& affinity) const
	{
		return new MemoryReadLatency(*this, affinity);
	}
}