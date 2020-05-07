//
// Created by klapeto on 19/4/20.
//

#include "Elpida/CommonTasks/AllocateMemory/AllocateMemorySpecification.hpp"
#include "Elpida/CommonTasks/AllocateMemory/AllocateMemory.hpp"
#include "Elpida/Engine/Configuration/TaskConfiguration.hpp"
#include "Elpida/Engine/Configuration/ConfigurationValue.hpp"
#include "Elpida/Engine/Configuration/ConfigurationSpecification.hpp"

namespace Elpida
{
	const char* AllocateMemorySpecification::memorySizeSetting = "Memory size";

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
		bool enableMultiThreading)
		: TaskSpecification("Allocate Memory",
		"Allocates memory to be used by other tasks on the benchmark",
		_noInputString.data(),
		_noInputString.data(),
		"Data that was allocated",
		"Bytes",
		"B",
		{
			new ConfigurationSpecification<ConfigurationType::UnsignedInt>(256ul, memorySizeSetting,
				"The amount of memory to allocate",
				true),
		},
		false,
		true,
		shouldBeCountedOnResults,
		enableMultiThreading,
		canBeDisabled)
	{
	}
}

