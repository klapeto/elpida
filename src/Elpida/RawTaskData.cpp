//
// Created by klapeto on 27/2/2023.
//

#include "Elpida/RawTaskData.hpp"

#include "Elpida/DataUtilities.hpp"

namespace Elpida
{

	RawTaskData::RawTaskData(const ProcessingUnitNode& targetProcessor)
		: _targetProcessor(targetProcessor), _size(0)
	{

	}

	void RawTaskData::Allocate(Size size)
	{
		_size = size;
		_data = NumaUniquePtr(GetNumaNodeId(), size);
		std::memset(_data.get(), 0, size);
	}

	RawTaskData::RawTaskData(RawTaskData&& other) noexcept
		: _targetProcessor(other._targetProcessor)
	{
		_data = std::move(other._data);
		_size = other._size;
		other._size = 0;
	}

	int RawTaskData::GetNumaNodeId() const
	{
		return NumaAllocator::GetProcessorNumaNode(_targetProcessor.get().GetOsIndex().value());
	}

	void RawTaskData::Merge(const Vector<UniquePtr<AbstractTaskData>>& data)
	{
		if (data.empty())
		{
			_size = 0;
			_data.reset();
			return;
		}

		Vector<Ref<const AbstractTaskData>> inputChunks;
		inputChunks.reserve(data.size());

		for (auto& chunk: data)
		{
			inputChunks.emplace_back(*chunk);
		}
		auto outputChunks = DataUtilities::SplitChunksToChunks<RawTaskData, RawTaskData>(inputChunks, {
			_targetProcessor }, 1, [](auto processor, auto chunkSize)
		{
			return std::make_unique<RawTaskData>(processor, chunkSize);
		});

		*this = std::move(*outputChunks.front());
	}

	RawTaskData& RawTaskData::operator=(RawTaskData&& other) noexcept
	{
		_targetProcessor = other._targetProcessor;
		_data = std::move(other._data);
		_size = other._size;
		other._size = 0;
		return *this;
	}

	void RawTaskData::Deallocate()
	{
		_data.reset();
		_size = 0;
	}

	void RawTaskData::Migrate(const ProcessingUnitNode& targetProcessor)
	{
		_targetProcessor = targetProcessor;
		auto newData = NumaUniquePtr(GetNumaNodeId(), _size);
		std::memcpy(newData.get(), _data.get(), _size);
		_data = std::move(newData);
	}

	Vector<UniquePtr<AbstractTaskData>>
	RawTaskData::Split(const Vector<Ref<const ProcessingUnitNode>>& targetProcessors) const
	{
		if (_size == 0)
		{
			Vector<UniquePtr<AbstractTaskData>> returnVector;
			returnVector.reserve(targetProcessors.size());

			for (auto& processor: targetProcessors)
			{
				returnVector.push_back(std::make_unique<RawTaskData>(processor.get()));
			}
			return returnVector;
		}

		return DataUtilities::SplitChunksToChunks<RawTaskData, AbstractTaskData>({ *this }, targetProcessors, 1,
			[](auto processor, auto chunkSize)
			{
				return std::make_unique<RawTaskData>(processor, chunkSize);
			});
	}

	unsigned char* RawTaskData::GetDataRaw() const
	{
		return _data.get();
	}

	Size RawTaskData::GetSize() const
	{
		return _size;
	}

	const ProcessingUnitNode& RawTaskData::GetTargetProcessor() const
	{
		return _targetProcessor;
	}
	RawTaskData::RawTaskData(const ProcessingUnitNode& targetProcessor, Size size)
		: RawTaskData(targetProcessor)
	{
		Allocate(size);
	}
} // Elpida