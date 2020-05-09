//
// Created by klapeto on 19/4/20.
//

#include "Elpida/CommonTasks/AllocateMemory/AllocateMemorySpecification.hpp"
#include "Elpida/CommonTasks/AllocateMemory/AllocateMemory.hpp"
#include "Elpida/Engine/Configuration/TaskConfiguration.hpp"
#include "Elpida/Engine/Configuration/ConfigurationValue.hpp"
#include "Elpida/Engine/Task/Data/DataSpecification.hpp"
#include "Elpida/Engine/Configuration/ConfigurationSpecification.hpp"

namespace Elpida
{

	Task* AllocateMemorySpecification::createNewTaskImpl(const TaskConfiguration& configuration,
		const TaskAffinity& affinity) const
	{
		auto size = getSettingAndValidate<size_t>(configuration,
			std::string(memorySizeSetting),
			ConfigurationType::Type::UnsignedInt);

		return new AllocateMemory(*this, affinity, shouldBeCountedOnResults(), size.getValue());
	}

	AllocateMemorySpecification::AllocateMemorySpecification(bool shouldBeCountedOnResults,
		bool canBeDisabled,
		bool enableMultiThreading, size_t fixedSize)
		: TaskSpecification("Allocate Memory",
		ResultSpecification("Allocation Rate", "B", ResultSpecification::Throughput, ResultSpecification::Accumulative))
	{
		withDescription("Allocates memory to be used by other tasks on the benchmark");

		setCanBeDisabled(canBeDisabled);
		setEnabledMultiThreading(enableMultiThreading);
		setToBeCountedOnResults(shouldBeCountedOnResults);

		withOutputData(DataSpecification("Allocated Bytes", "B", "The allocated memory"));

		auto config = new ConfigurationSpecification<ConfigurationType::UnsignedInt>(256ul,
			memorySizeSetting,
			"The amount of memory to allocate",
			true);
		if (fixedSize > 0)
		{
			withFixedConfiguration(config, fixedSize);
		}
		else
		{
			withConfiguration(config);
		}
	}
}

