//
// Created by klapeto on 28/4/20.
//

#ifndef SRC_BENCHMARKS_MEMORY_READ_MEMORYREADSPECIFICATION_HPP
#define SRC_BENCHMARKS_MEMORY_READ_MEMORYREADSPECIFICATION_HPP

#include <Elpida/Engine/Task/TaskSpecification.hpp>

namespace Elpida
{
	class MemoryReadSpecification : public TaskSpecification
	{
	public:
		MemoryReadSpecification(bool shouldBeCountedOnResults, bool canBeDisabled, bool enableMultiThreading);

		~MemoryReadSpecification() override = default;

	protected:
		[[nodiscard]] Task* createNewTaskImpl(const TaskConfiguration& configuration,
			const TaskAffinity& affinity) const override;
	};
}


#endif //SRC_BENCHMARKS_MEMORY_READ_MEMORYREADSPECIFICATION_HPP
