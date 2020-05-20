//
// Created by klapeto on 10/5/20.
//

#include <cstring>
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
		const auto& taskData = output.getTaskData();
		auto accumulatedSize = getAccumulatedSizeOfChunks(taskData);
		while (accumulatedSize % inputDataSpecification.getSizeShouldBeDivisibleBy() != 0)
		{
			accumulatedSize++;
		}
		auto newTaskData = new ActiveTaskData(accumulatedSize,
			SystemTopology::getNumaNodeOfProcessor((int)affinity.getProcessors(1).front()->getOsIndex()));

		size_t offset = 0;
		for (const auto& chunk: taskData)
		{
			std::memcpy(newTaskData->getData() + offset, chunk->getData(), chunk->getSize());
			offset += chunk->getSize();
		}

		return TaskInput({ newTaskData });
	}
}