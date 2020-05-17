//
// Created by klapeto on 10/5/20.
//

#include "Elpida/Engine/Data/Adapters/ForwardingAdapter.hpp"
#include "Elpida/Engine/Task/TaskOutput.hpp"

namespace Elpida
{

	TaskInput ForwardingAdapter::transformOutputToInput(const TaskOutput& output,
		const TaskAffinity& affinity,
		const DataSpecification& inputDataSpecification) const
	{
		return TaskInput(output.createPassiveWrappers());
	}
}