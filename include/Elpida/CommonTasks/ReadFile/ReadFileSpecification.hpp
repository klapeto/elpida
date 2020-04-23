//
// Created by klapeto on 20/4/20.
//

#ifndef INCLUDE_ELPIDA_COMMONTASKS_READFILE_READFILESPECIFICATION_HPP
#define INCLUDE_ELPIDA_COMMONTASKS_READFILE_READFILESPECIFICATION_HPP

#include "Elpida/Engine/Task/TaskSpecification.hpp"

namespace Elpida
{
	class ReadFileSpecification final : public TaskSpecification
	{
	public:
		static const char* filePathSetting;

		[[nodiscard]] Task* createNewTask(const TaskConfiguration& configuration,
			const TaskAffinity& affinity) const override;

		ReadFileSpecification(bool shouldBeCountedOnResults, bool canBeDisabled);
		~ReadFileSpecification() override = default;
	};
}


#endif //INCLUDE_ELPIDA_COMMONTASKS_READFILE_READFILESPECIFICATION_HPP
