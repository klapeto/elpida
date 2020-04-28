//
// Created by klapeto on 28/4/20.
//

#include "Benchmarks/Memory/Read/MemoryReadSpecification.hpp"
#include "Benchmarks/Memory/Read/MemoryRead.hpp"

namespace Elpida
{

	MemoryReadSpecification::MemoryReadSpecification(
		bool shouldBeCountedOnResults,
		bool canBeDisabled,
		bool enableMultiThreading)
		: TaskSpecification("Memory Read Bandwidth",
		"Reads continuously memory regions provided to measure peak bandwidth",
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
	Task* MemoryReadSpecification::createNewTask(const TaskConfiguration& configuration,
		const TaskAffinity& affinity) const
	{
		return new MemoryRead(*this, affinity, shouldBeCountedOnResults());
	}
}