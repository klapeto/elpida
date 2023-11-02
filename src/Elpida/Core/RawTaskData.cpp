//
// Created by klapeto on 27/2/2023.
//

#include "Elpida/Core/RawTaskData.hpp"

#include "Elpida/Core/DataUtilities.hpp"

namespace Elpida
{

	RawTaskData::RawTaskData(const ProcessingUnitNode& targetProcessor, const Allocator& allocator)
		: _data(nullptr, Deleter(allocator, 0)), _targetProcessor(targetProcessor), _allocator(allocator), _size(0)
	{

	}

	void RawTaskData::Allocate(Size size)
	{
		_size = size;
		_data = UniquePtr<unsigned char, Deleter>(
			(unsigned char*)_allocator.get().Allocate(_targetProcessor.get(), size),
			Deleter(_allocator, size));
		std::memset(_data.get(), 0, size);
	}

	RawTaskData::RawTaskData(RawTaskData&& other) noexcept
		: _data(std::move(other._data)), _targetProcessor(other._targetProcessor), _allocator(other._allocator)
	{
		_size = other._size;
		other._size = 0;
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
			_targetProcessor }, 1, [this](auto processor, auto chunkSize)
		{
			return std::make_unique<RawTaskData>(processor, chunkSize, _allocator.get());
		});

		*this = std::move(*outputChunks.front());
	}

	RawTaskData& RawTaskData::operator=(RawTaskData&& other) noexcept
	{
		_allocator = other._allocator;
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

	Vector<UniquePtr<AbstractTaskData>>
	RawTaskData::Split(const Vector<Ref<const ProcessingUnitNode>>& targetProcessors) const
	{
		if (_size == 0)
		{
			Vector<UniquePtr<AbstractTaskData>> returnVector;
			returnVector.reserve(targetProcessors.size());

			for (auto& processor: targetProcessors)
			{
				returnVector.push_back(std::make_unique<RawTaskData>(processor.get(), _allocator));
			}
			return returnVector;
		}

		return DataUtilities::SplitChunksToChunks<RawTaskData, AbstractTaskData>({ *this }, targetProcessors, 1,
			[this](auto processor, auto chunkSize)
			{
				return std::make_unique<RawTaskData>(processor, chunkSize, _allocator);
			});
	}

	unsigned char* RawTaskData::GetData() const
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

	RawTaskData::RawTaskData(const ProcessingUnitNode& targetProcessor, Size size, const Allocator& allocator)
		: RawTaskData(targetProcessor, allocator)
	{
		Allocate(size);
	}

	const Allocator& RawTaskData::GetAllocator() const
	{
		return _allocator.get();
	}
} // Elpida