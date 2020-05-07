//
// Created by klapeto on 30/4/20.
//

#include "Benchmarks/Memory/Latency/MemoryReadLatencySpecification.hpp"
#include "Benchmarks/Memory/Latency/MemoryReadLatency.hpp"

namespace Elpida
{

	MemoryReadLatencySpecification::MemoryReadLatencySpecification(bool shouldBeCountedOnResults,
		bool canBeDisabled,
		bool enableMultiThreading)
		: TaskSpecification("Memory Read Latency",
		"Reads random positions of continuous memory to measure worst read latency",
		"Memory regions to read",
		"bytes",
		"Memory regions to that were read",
		"bytes",
		"B",
		{},
		true,
		true,
		shouldBeCountedOnResults,
		enableMultiThreading,
		canBeDisabled)
	{

	}
	Task* MemoryReadLatencySpecification::createNewTaskImpl(const TaskConfiguration& configuration,
		const TaskAffinity& affinity) const
	{
		return new MemoryReadLatency(*this, affinity, false);
	}
}