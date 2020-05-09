//
// Created by klapeto on 19/4/20.
//

#ifndef INCLUDE_ELPIDA_COMMONTASKS_ALLOCATEMEMORY_ALLOCATEMEMORYSPECIFICATION_HPP
#define INCLUDE_ELPIDA_COMMONTASKS_ALLOCATEMEMORY_ALLOCATEMEMORYSPECIFICATION_HPP

#include "Elpida/Engine/Task/TaskSpecification.hpp"

namespace Elpida
{
	class AllocateMemorySpecification final : public TaskSpecification
	{
	public:
		static inline const char* memorySizeSetting = "Memory size";

		[[nodiscard]] Task* createNewTaskImpl(const TaskConfiguration& configuration,
			const TaskAffinity& affinity) const override;

		AllocateMemorySpecification(bool shouldBeCountedOnResults, bool canBeDisabled, bool enableMultiThreading, size_t fixedSize = 0);
		~AllocateMemorySpecification() override = default;
	};
}


#endif //INCLUDE_ELPIDA_COMMONTASKS_ALLOCATEMEMORY_ALLOCATEMEMORYSPECIFICATION_HPP
