//
// Created by klapeto on 20/4/20.
//

#ifndef INCLUDE_ELPIDA_COMMONTASKS_WRITEFILE_WRITEFILESPECIFICATION_HPP
#define INCLUDE_ELPIDA_COMMONTASKS_WRITEFILE_WRITEFILESPECIFICATION_HPP

#include "Elpida/Engine/Task/TaskSpecification.hpp"

namespace Elpida
{
	class WriteFileSpecification final : public TaskSpecification
	{
	public:
		class Settings
		{
		public:
			static inline const char* OutputFilePath = "Output File Name";
		};

		static inline constexpr const char* Uuid = "a7b5315b-686b-40be-addc-4102843f6024";

		[[nodiscard]]
		std::unique_ptr<Task> createNewTask(const TaskConfiguration& configuration,
			const ProcessorNode& processorToRun,
			const ServiceProvider& serviceProvider,
			size_t iterationsToRun) const override;

		WriteFileSpecification();
		~WriteFileSpecification() override = default;
	};
}

#endif //INCLUDE_ELPIDA_COMMONTASKS_WRITEFILE_WRITEFILESPECIFICATION_HPP
