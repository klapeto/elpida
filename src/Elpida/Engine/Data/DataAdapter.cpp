/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2020  Ioannis Panagiotopoulos
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>
 *************************************************************************/

//
// Created by klapeto on 10/5/20.
//

#include "Elpida/Engine/Data/DataAdapter.hpp"

#include "Elpida/Engine/Task/TaskAffinity.hpp"
#include "Elpida/Engine/Data/RawTaskData.hpp"
#include "Elpida/SystemInfo/SystemTopology.hpp"
#include "Elpida/SystemInfo/ProcessorNode.hpp"
#include "Elpida/Config.hpp"

#include <cstring>
#include <vector>
#include <cmath>

namespace Elpida
{

	std::vector<std::unique_ptr<RawTaskData>> DataAdapter::breakIntoChunksImpl(
		const std::vector<const RawTaskData*>& input,
		const std::vector<const ProcessorNode*>& processors,
		size_t chunksDivisibleBy)
	{
		auto targetChunksCount = processors.size();
		auto& outputChunks = input;

		auto outputTotalSize = getAccumulatedSizeOfChunks(input);

		size_t targetChunkSize = std::ceil(outputTotalSize / (float)targetChunksCount);

		while (targetChunkSize % chunksDivisibleBy != 0)
		{
			targetChunkSize++;
		}

		std::vector<std::unique_ptr<RawTaskData>> targetChunksVec;
		targetChunksVec.reserve(targetChunksCount);

		size_t currentChunkIndex = 0;
		const RawTaskData* currentChunk = nullptr;
		size_t currentChunkOffset = 0;
		size_t currentChunkSize = 0;
		size_t totalDataCopied = 0;
		for (const auto& outChunk : outputChunks)
		{
			size_t oChunkSize = outChunk->getSize();
			size_t oChunkBytesCopied = 0;

			// while we haven't done copying this output chunk
			while (oChunkBytesCopied < oChunkSize)
			{
				if (currentChunk == nullptr)
				{
					currentChunkSize = std::min(targetChunkSize, outputTotalSize - totalDataCopied);
					// create new chunk with the defined target size
					targetChunksVec.push_back(std::make_unique<RawTaskData>(currentChunkSize, *processors[currentChunkIndex]));
					currentChunk = targetChunksVec.back().get();
				}

				size_t actualReadSize;
				if (oChunkSize - oChunkBytesCopied > currentChunkSize - currentChunkOffset)
				{
					// Does not fit to current target chunk. This means we need to read UP TO the rest of the target
					// chunk capacity
					actualReadSize = currentChunkSize - currentChunkOffset;
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

				totalDataCopied += actualReadSize;

				if (currentChunkOffset >= currentChunkSize)
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

	std::vector<std::unique_ptr<RawTaskData>> DataAdapter::breakIntoChunks(const std::vector<const RawTaskData*>& inputData,
		const TaskAffinity& affinity,
		size_t divisibleBy)
	{
		return breakIntoChunksImpl(inputData, affinity.getProcessorNodes(), divisibleBy);
	}

	std::unique_ptr<RawTaskData> DataAdapter::mergeIntoSingleChunk(const std::vector<const RawTaskData*>& inputData, const ProcessorNode& processor)
	{
		return std::move(breakIntoChunksImpl(inputData, { &processor }, 1).front());
	}

	size_t DataAdapter::getAccumulatedSizeOfChunks(const std::vector<const RawTaskData*>& outputChunks)
	{
		size_t outputTotalSize = 0;
		for (auto& data : outputChunks)
		{
			outputTotalSize += data->getSize();
		}
		return outputTotalSize;
	}
}