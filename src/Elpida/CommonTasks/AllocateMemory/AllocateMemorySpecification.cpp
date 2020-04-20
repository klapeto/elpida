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

	Task* AllocateMemorySpecification::createNewTask(const TaskConfiguration& configuration,
		const TaskAffinity& affinity,
		bool toBeCountedOnResults) const
	{
		auto size = getSettingAndValidate<size_t>(configuration,
			memorySizeSetting.data(),
			ConfigurationValueBase::Type::UnsignedInt);

		return new AllocateMemory(*this, affinity, toBeCountedOnResults, size.getValue());
	}

	AllocateMemorySpecification::AllocateMemorySpecification(bool isToBeMeasured)
		: TaskSpecification("Allocate Memory",
		"Allocates memory to be used by other tasks on the benchmark",
		_noInputString.data(),
		_noInputString.data(),
		"Data that was allocated",
		"Bytes",
		"B",
		{
			ConfigurationSpecification("Memory size",
				"The amount of memory to allocate",
				ConfigurationValueBase::Type::UnsignedInt),
		}, isToBeMeasured)
	{
	}
}

