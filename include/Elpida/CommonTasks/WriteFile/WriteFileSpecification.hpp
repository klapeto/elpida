//
// Created by klapeto on 20/4/20.
//

#ifndef INCLUDE_ELPIDA_COMMONTASKS_WRITEFILE_WRITEFILESPECIFICATION_HPP
#define INCLUDE_ELPIDA_COMMONTASKS_WRITEFILE_WRITEFILESPECIFICATION_HPP

#include "Elpida/Engine/Task/TaskSpecification.hpp"

namespace Elpida {
	class WriteFileSpecification final : public TaskSpecification
	{
	public:
		static const char* filePathSetting;

		[[nodiscard]] Task* createNewTask(const TaskConfiguration& configuration,
			const TaskAffinity& affinity) const override;

		WriteFileSpecification();
		~WriteFileSpecification() override = default;
	};
}

#endif //INCLUDE_ELPIDA_COMMONTASKS_WRITEFILE_WRITEFILESPECIFICATION_HPP
