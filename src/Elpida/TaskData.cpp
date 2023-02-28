//
// Created by klapeto on 27/2/2023.
//

#include "TaskData.hpp"

#include <cstring>
#include <cmath>

namespace Elpida
{

	TaskData::TaskData(int processorId)
		: _processorId(processorId), _size(0)
	{
	}

	TaskData::TaskData()
		: _processorId(0), _size(0)
	{

	}

	void TaskData::Allocate(std::size_t size)
	{
		_size = size;
		_data = NumaUniquePtr(GetNumaNodeId(), size);
		std::memset(_data.get(), 0, size);
	}

	void TaskData::Migrate(int processorId)
	{
		_processorId = processorId;

		auto newData = NumaUniquePtr(GetNumaNodeId(), _size);

		std::memcpy(newData.get(), _data.get(), _size);

		_data = std::move(newData);
	}

	TaskData::TaskData(TaskData&& other) noexcept
	{
		_data = std::move(other._data);
		_processorId = other._processorId;
		_size = other._size;
		other._size = 0;
	}

	int TaskData::GetNumaNodeId() const
	{
		return NumaAllocator::GetProcessorNumaNode(_processorId);
	}

	std::vector<TaskData> TaskData::Split(const std::vector<int>& processorIds, std::size_t chunkDivisibleBy)
	{
		if (!_data)
		{
			std::vector<TaskData> returnData;
			returnData.reserve(processorIds.size());
			for (auto processorId: processorIds)
			{
				returnData.emplace_back(processorId);
			}
			return returnData;
		}
		return SplitChunksToChunks({ this }, processorIds, chunkDivisibleBy);
	}

	void TaskData::Merge(const std::vector<TaskData>& chunks)
	{
		std::vector<const TaskData*> chunksPointers;

		for (auto& chunk: chunks)
		{
			chunksPointers.push_back(&chunk);
		}

		auto outputChunks = SplitChunksToChunks(chunksPointers, { _processorId }, 1);

		*this = std::move(outputChunks.front());
	}

	TaskData& TaskData::operator=(TaskData&& other) noexcept
	{
		_data = std::move(other._data);
		_processorId = other._processorId;
		_size = other._size;
		other._size = 0;
		return *this;
	}

	static std::size_t GetAccumulatedSizeOfChunks(const std::vector<const TaskData*>& outputChunks)
	{
		std::size_t outputTotalSize = 0;
		for (const auto& data: outputChunks)
		{
			outputTotalSize += data->GetSize();
		}
		return outputTotalSize;
	}

	std::vector<TaskData>
	TaskData::SplitChunksToChunks(const std::vector<const TaskData*>& input, const std::vector<int>& processorIds, std::size_t chunkDivisibleBy)
	{
		auto targetChunksCount = processorIds.size();
		auto& outputChunks = input;

		auto outputTotalSize = GetAccumulatedSizeOfChunks(input);

		std::size_t targetChunkSize = std::ceil((float)outputTotalSize / (float)targetChunksCount);

		while (targetChunkSize % chunkDivisibleBy != 0)
		{
			targetChunkSize++;
		}

		std::vector<TaskData> targetChunksVec;
		targetChunksVec.reserve(targetChunksCount);

		size_t currentChunkIndex = 0;
		const TaskData* currentChunk = nullptr;
		size_t currentChunkOffset = 0;
		size_t currentChunkSize = 0;
		size_t totalDataCopied = 0;
		for (const auto& outChunk: outputChunks)
		{
			size_t oChunkSize = outChunk->GetSize();
			size_t oChunkBytesCopied = 0;

			// while we haven't done copying this output chunk
			while (oChunkBytesCopied < oChunkSize)
			{
				if (currentChunk == nullptr)
				{
					currentChunkSize = std::min(targetChunkSize, outputTotalSize - totalDataCopied);

					// create new chunk with the defined target size
					auto chunk = TaskData(processorIds[currentChunkIndex]);
					chunk.Allocate(currentChunkSize);
					targetChunksVec.push_back(std::move(chunk));

					currentChunk = &targetChunksVec.back();
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
				std::memcpy(currentChunk->GetDataRaw() + currentChunkOffset,
					outChunk->GetDataRaw() + oChunkBytesCopied,
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

	void TaskData::Deallocate()
	{
		_data.reset();
		_size = 0;
	}
} // Elpida