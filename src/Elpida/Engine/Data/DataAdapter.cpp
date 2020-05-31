//
// Created by klapeto on 10/5/20.
//

#include "Elpida/Engine/Data/DataAdapter.hpp"


#include "Elpida/Engine/Task/TaskAffinity.hpp"
#include "Elpida/Engine/Data/ActiveTaskData.hpp"
#include "Elpida/Topology/SystemTopology.hpp"
#include "Elpida/Topology/ProcessorNode.hpp"
#include "Elpida/Engine/Data/DataSpecification.hpp"
#include "Elpida/Engine/Task/TaskSpecification.hpp"
#include "Elpida/Config.hpp"
#include "Elpida/Engine/Task/Task.hpp"
#include "Elpida/Engine/Data/DataPropertiesTransformer.hpp"

#include <cstring>
#include <vector>
#include <cmath>


namespace Elpida
{

	std::vector<RawData*> DataAdapter::breakIntoChunksImpl(const std::vector<const RawData*>& input,
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

	std::vector<RawData*> DataAdapter::breakIntoChunks(const RawData& input,
		const TaskAffinity& affinity,
		const DataSpecification& inputDataSpecification)
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

	RawData* DataAdapter::mergeIntoSingleChunk(const std::vector<const RawData*>& inputData)
	{
		return breakIntoChunksImpl(inputData, { 0 }, 1).front();
	}

	size_t DataAdapter::getAccumulatedSizeOfChunks(const std::vector<const RawData*>& outputChunks)
	{
		size_t outputTotalSize = 0;
		for (auto& data : outputChunks)
		{
			outputTotalSize += data->getSize();
		}
		return outputTotalSize;
	}

	void DataAdapter::adaptAndForwardTaskData(Task* previous, Task* next)
	{
		if (previous != nullptr)
		{
			if (next != nullptr && next->getSpecification().acceptsInput())
			{
				auto& nextSpec = next->getSpecification();
				auto& prevOutput = previous->getOutput();

				auto chunks = breakIntoChunksImpl({ prevOutput.getTaskData() },
					{ 0 },
					nextSpec.getInputDataSpecification().getSizeShouldBeDivisibleBy());

				auto propertiesTransformer = nextSpec.getDataPropertiesTransformer();
				if (propertiesTransformer != nullptr)
				{
					auto& chunk = *chunks.front();
					next->setInput(TaskDataDto(chunk,
						propertiesTransformer->transform(prevOutput.getTaskData()->getSize(),
							prevOutput.getDefinedProperties(),
							chunk.getSize())));
				}
				else
				{
					next->setInput(TaskDataDto(*chunks.front(), prevOutput.getDefinedProperties()));
				}

			}

			auto prevInput = previous->getInput().getTaskData();
			auto prevOutput = previous->getOutput().getTaskData();
			delete prevInput;
			if (prevInput != prevOutput)
			{
				delete prevOutput;
			}
			previous->setInput(TaskDataDto());
		}
	}
}