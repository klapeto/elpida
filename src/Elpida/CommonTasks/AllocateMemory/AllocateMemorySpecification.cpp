//
// Created by klapeto on 19/4/20.
//

#include "Elpida/CommonTasks/AllocateMemory/AllocateMemorySpecification.hpp"
#include "Elpida/CommonTasks/AllocateMemory/AllocateMemory.hpp"
#include "Elpida/Engine/Configuration/Concrete/TaskConfiguration.hpp"
#include "Elpida/Engine/Configuration/Concrete/ConfigurationValue.hpp"
#include "Elpida/Engine/Data/DataSpecification.hpp"
#include "Elpida/Engine/Configuration/Specification/ConfigurationSpecification.hpp"

namespace Elpida
{

	Task* AllocateMemorySpecification::createNewTask(const TaskConfiguration& configuration,
		const ProcessorNode& processorToRun) const
	{
		auto size = getSettingAndValidate<size_t>(configuration,
			std::string(Settings::MemorySize),
			ConfigurationType::Type::UnsignedInt);

		return new AllocateMemory(*this, processorToRun, size.getValue());
	}

	AllocateMemorySpecification::AllocateMemorySpecification()
		: TaskSpecification("Allocate Memory",
		ResultSpecification("Allocation Rate", "B", ResultSpecification::Throughput, ResultSpecification::Accumulative))
	{
		withDescription("Allocates memory to be used by other tasks on the benchmark");

		withOutputData(DataSpecification("Allocated Bytes", "B", "The allocated memory"));

		withConfiguration(new ConfigurationSpecification<ConfigurationType::UnsignedInt>(256ul,
			Settings::MemorySize,
			"The amount of memory to allocate",
			true));
	}
}

