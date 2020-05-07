//
// Created by klapeto on 30/4/20.
//

#ifndef SRC_BENCHMARKS_MEMORY_LATENCY_MEMORYLATENCYSPECIFICATION_HPP
#define SRC_BENCHMARKS_MEMORY_LATENCY_MEMORYLATENCYSPECIFICATION_HPP

#include <Elpida/Engine/Task/TaskSpecification.hpp>

namespace Elpida {

	class MemoryReadLatencySpecification: public TaskSpecification
	{
	public:
		MemoryReadLatencySpecification(bool shouldBeCountedOnResults, bool canBeDisabled, bool enableMultiThreading);
		~MemoryReadLatencySpecification() override = default;

	protected:
		[[nodiscard]] Task* createNewTaskImpl(const TaskConfiguration& configuration,
			const TaskAffinity& affinity) const override;
	};
}

#endif //SRC_BENCHMARKS_MEMORY_LATENCY_MEMORYLATENCYSPECIFICATION_HPP
