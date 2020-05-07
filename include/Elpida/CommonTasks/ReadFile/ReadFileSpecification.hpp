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

		[[nodiscard]] Task* createNewTaskImpl(const TaskConfiguration& configuration,
			const TaskAffinity& affinity) const override;

		ReadFileSpecification(bool shouldBeCountedOnResults, bool canBeDisabled, const std::string& defaultValue = std::string());
		~ReadFileSpecification() override = default;
	};
}


#endif //INCLUDE_ELPIDA_COMMONTASKS_READFILE_READFILESPECIFICATION_HPP
