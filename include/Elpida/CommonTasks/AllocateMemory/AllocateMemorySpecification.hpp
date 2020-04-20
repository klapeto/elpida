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
		std::string_view memorySizeSetting = "memorySize";

		[[nodiscard]] Task* createNewTask(const TaskConfiguration& configuration,
			const TaskAffinity& affinity) const override;

		AllocateMemorySpecification(bool toBeMeasured);
		~AllocateMemorySpecification() override = default;
	};
}


#endif //INCLUDE_ELPIDA_COMMONTASKS_ALLOCATEMEMORY_ALLOCATEMEMORYSPECIFICATION_HPP
