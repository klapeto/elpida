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
		std::string filePathSetting = "filePath";

		[[nodiscard]] Task* createNewTask(const TaskConfiguration& configuration,
			const TaskAffinity& affinity,
			const std::string& settingsNamespace,
			bool toBeCountedOnResults) const override;

		ReadFileSpecification();
		~ReadFileSpecification() = default;
	};
}


#endif //INCLUDE_ELPIDA_COMMONTASKS_READFILE_READFILESPECIFICATION_HPP
