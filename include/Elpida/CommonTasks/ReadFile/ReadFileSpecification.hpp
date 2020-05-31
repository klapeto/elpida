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
		class Settings
		{
		public:
			static inline const char* InputFilePath = "Input File Name";
		};

		[[nodiscard]] Task* createNewTask(const TaskConfiguration& configuration,
			const ProcessorNode& processorToRun) const override;

		ReadFileSpecification();
		~ReadFileSpecification() override = default;
	};
}


#endif //INCLUDE_ELPIDA_COMMONTASKS_READFILE_READFILESPECIFICATION_HPP
