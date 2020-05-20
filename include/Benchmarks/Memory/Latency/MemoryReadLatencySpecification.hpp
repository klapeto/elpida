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

		[[nodiscard]] Task* createNewTask(const TaskConfiguration& configuration, const TaskAffinity& affinity) const override;

		MemoryReadLatencySpecification();
		~MemoryReadLatencySpecification() override = default;
	};
}

#endif //SRC_BENCHMARKS_MEMORY_LATENCY_MEMORYLATENCYSPECIFICATION_HPP
