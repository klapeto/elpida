//
// Created by klapeto on 28/4/20.
//

#include "Benchmarks/Memory/Read/MemoryReadSpecification.hpp"
#include "Benchmarks/Memory/Read/MemoryRead.hpp"

namespace Elpida
{

	MemoryReadSpecification::MemoryReadSpecification()
		: TaskSpecification("Memory Read Bandwidth",
		ResultSpecification("Read Rate", "B", ResultSpecification::Throughput, ResultSpecification::Accumulative))
	{
		withDescription("Reads continuously a memory region to determine Memory Read Bandwidth");
		withInputData(DataSpecification("Input Memory",
			"B",
			32 * sizeof(RegisterSize),
			"The allocated memory region to read"));
	}

	Task* MemoryReadSpecification::createNewTask(const TaskConfiguration& configuration,
		const ProcessorNode& processorToRun) const
	{
		return new MemoryRead(*this, processorToRun);
	}
}