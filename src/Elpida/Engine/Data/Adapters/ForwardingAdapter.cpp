//
// Created by klapeto on 10/5/20.
//

#include "Elpida/Engine/Data/Adapters/ForwardingAdapter.hpp"
#include "Elpida/Engine/Data/PassiveTaskData.hpp"
#include "Elpida/Engine/Task/TaskAffinity.hpp"
#include "Elpida/Engine/Data/DataSpecification.hpp"
#include "Elpida/Config.hpp"
#include "Elpida/ElpidaException.hpp"

#include <cmath>

namespace Elpida
{

	std::vector<RawData*> ForwardingAdapter::breakIntoChunks(const RawData& input,
		const TaskAffinity& affinity,
		const DataSpecification& inputDataSpecification) const
	{
		std::vector<RawData*> returnVector;

		auto targetChunksCount = affinity.getProcessorNodes().size();
		auto inputSize = input.getSize();

		size_t targetChunkSize = std::ceil(inputSize / (float)targetChunksCount);    // calculate the initial chunk size

		const auto divisibleBy = inputDataSpecification.getSizeShouldBeDivisibleBy();
		while (targetChunkSize % divisibleBy != 0)    // Fix the chunk size to match what the spec wants
		{
			targetChunkSize++;
		}

		targetChunksCount = std::ceil(inputSize / (float)targetChunkSize);    // recalculate the chunk count

		returnVector.reserve(targetChunksCount);

		auto curOffset = 0ul;
		for (auto i = 0ul; i < targetChunksCount; ++i)
		{
			auto actualChunkSize = std::min(inputSize - (targetChunkSize * i), targetChunkSize);
			returnVector.push_back(new PassiveTaskData(input.getData() + curOffset, actualChunkSize));
			curOffset += actualChunkSize;
		}
		return returnVector;
	}

	RawData* ForwardingAdapter::mergeIntoSingleChunk(const std::vector<const RawData*>& inputData) const
	{
		auto curData = inputData.front();
		unsigned char* startPtr = curData->getData();
		unsigned char* curPtr = startPtr;

		size_t totalSize = 0;
		bool first = true;
		for (auto chunk: inputData)
		{
			if (!first)
			{
				auto thisPtr = chunk->getData();
				if (thisPtr == (curPtr + curData->getSize()))
				{
					curPtr = thisPtr;
					totalSize += chunk->getSize();
				}
				else
				{
					throw ElpidaException(FUNCTION_NAME,
						"Non contiguous chunks were provided to ForwardingAdapter. Maybe mixed adapters were used for breaking/merging memory.");
				}
			}
			else
			{
				first = false;
				continue;
			}
		}
		return new PassiveTaskData(startPtr, totalSize);
	}
}