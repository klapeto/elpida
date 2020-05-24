//
// Created by klapeto on 10/5/20.
//


#include "Elpida/Engine/Data/Adapters/CopyingAdapter.hpp"

#include "Elpida/Engine/Task/TaskAffinity.hpp"
#include "Elpida/Engine/Data/ActiveTaskData.hpp"
#include "Elpida/Topology/SystemTopology.hpp"
#include "Elpida/Topology/ProcessorNode.hpp"
#include "Elpida/Engine/Data/DataSpecification.hpp"
#include "Elpida/Config.hpp"

#include <cstring>
#include <vector>
#include <cmath>

namespace Elpida
{
	std::vector<RawData*> CopyingAdapter::breakIntoChunksImpl(const std::vector<const RawData*>& input,
		const std::vector<int>& processorsOsIndices,
		size_t chunksDivisibleBy)
	{
		auto targetChunksCount = processorsOsIndices.size();
		auto& outputChunks = input;

		auto outputTotalSize = getAccumulatedSizeOfChunks(input);

		size_t targetChunkSize = std::ceil(outputTotalSize / (float)targetChunksCount);

		while (targetChunkSize % chunksDivisibleBy != 0)
		{
			targetChunkSize++;
		}

		std::vector<RawData*> targetChunksVec;
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
						SystemTopology::getNumaNodeOfProcessor(processorsOsIndices[currentChunkIndex]));
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
		return targetChunksVec;
	}

	std::vector<RawData*> CopyingAdapter::breakIntoChunks(const RawData& input,
		const TaskAffinity& affinity,
		const DataSpecification& inputDataSpecification) const
	{
		std::vector<int> processorsOsIndices;
		for (auto processor : affinity.getProcessorNodes())
		{
			processorsOsIndices.push_back(processor->getOsIndex());
		}
		return breakIntoChunksImpl({ &input },
			processorsOsIndices,
			inputDataSpecification.getSizeShouldBeDivisibleBy());
	}

	RawData* CopyingAdapter::mergeIntoSingleChunk(const std::vector<const RawData*>& inputData) const
	{
		return breakIntoChunksImpl(inputData, { 0 }, 1).front();
	}
}