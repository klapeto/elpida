//
// Created by klapeto on 10/5/20.
//


#include "Elpida/Engine/Data/Adapters/CopyingChunkNormalizerAdapter.hpp"

#include "Elpida/Engine/Task/TaskAffinity.hpp"
#include "Elpida/Engine/Data/ActiveTaskData.hpp"
#include "Elpida/Engine/Task/TaskOutput.hpp"
#include "Elpida/Topology/SystemTopology.hpp"
#include "Elpida/Topology/ProcessorNode.hpp"

#include <cstring>
#include <cmath>

namespace Elpida
{

	TaskInput CopyingChunkNormalizerAdapter::transformOutputToInput(const TaskOutput& output,
		const TaskAffinity& affinity) const
	{
		auto& processors = affinity.getProcessorNodes();
		auto targetChunksCount = processors.size();
		auto& outputChunks = output.getTaskData();

		auto outputTotalSize = getAccumulatedSizeOfChunks(outputChunks);

		size_t targetChunkSize = std::ceil(outputTotalSize / (float)targetChunksCount);

		std::vector<TaskData*> targetChunksVec;
		targetChunksVec.reserve(targetChunksCount);

		size_t currentChunkIndex = 0;
		ActiveTaskData* currentChunk = nullptr;
		size_t currentChunkOffset = 0;
		for (auto outChunk: outputChunks)
		{
			size_t oChunkSize = outChunk->getSize();
			size_t oChunkBytesCopied = 0;

			// while we haven't done copying this output chunk
			while (oChunkBytesCopied < oChunkSize)
			{
				if (currentChunk == nullptr)
				{
					// create new chunk with the defined target size
					currentChunk = new ActiveTaskData(targetChunkSize,
						SystemTopology::getNumaNodeOfProcessor((int)processors[currentChunkIndex]->getOsIndex()));
					targetChunksVec.push_back(currentChunk);
				}

				size_t actualReadSize;
				if (oChunkSize - oChunkBytesCopied > targetChunkSize - currentChunkOffset)
				{
					// Does not fit to current target chunk. This means we need to read UP TO the rest of the target
					// chunk capacity
					actualReadSize = targetChunkSize - currentChunkOffset;
				}
				else
				{
					// Fits to current target chunk. This means that we need to read the remaining bytes of the output
					// chunk
					actualReadSize = oChunkSize - oChunkBytesCopied;
				}

				// copy the data
				std::memcpy(currentChunk->getData() + currentChunkOffset,
					outChunk->getData() + oChunkBytesCopied,
					actualReadSize);

				// add offsets of the actual copied data
				oChunkBytesCopied += actualReadSize;
				currentChunkOffset += actualReadSize;

				if (currentChunkOffset >= targetChunkSize)
				{
					// trigger a chunk creation on next iteration
					currentChunk = nullptr;
					currentChunkOffset = 0;
					currentChunkIndex++;
				}
			}
		}

		return TaskInput(std::move(targetChunksVec));
	}
}