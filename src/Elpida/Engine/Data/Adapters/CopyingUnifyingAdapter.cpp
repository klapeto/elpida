//
// Created by klapeto on 10/5/20.
//

#include "Elpida/Engine/Data/Adapters/CopyingUnifyingAdapter.hpp"
#include "Elpida/Engine/Data/ActiveTaskData.hpp"
#include "Elpida/Engine/Data/DataSpecification.hpp"
#include "Elpida/Engine/Task/TaskOutput.hpp"
#include "Elpida/Topology/SystemTopology.hpp"
#include "Elpida/Topology/ProcessorNode.hpp"
#include "Elpida/Engine/Task/TaskAffinity.hpp"

namespace Elpida
{

	TaskInput CopyingUnifyingAdapter::transformOutputToInput(const TaskOutput& output,
		const TaskAffinity& affinity,
		const DataSpecification& inputDataSpecification) const
	{
		auto accumulatedSize = getAccumulatedSizeOfChunks(output.getTaskData());
		while (accumulatedSize % inputDataSpecification.getSizeShouldBeDivisibleBy() != 0)
		{
			accumulatedSize++;
		}
		auto taskData = new ActiveTaskData(accumulatedSize,
			SystemTopology::getNumaNodeOfProcessor((int)affinity.getProcessors(1).front()->getOsIndex()));
		return TaskInput({ taskData });
	}
}