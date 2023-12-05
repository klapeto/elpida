//
// Created by klapeto on 27/2/2023.
//

#include "Elpida/Core/RawTaskData.hpp"

#include "Elpida/Core/Allocator.hpp"
#include "Elpida/Core/DataUtilities.hpp"
#include "Elpida/Core/SharedPtr.hpp"

namespace Elpida
{

	RawTaskData::RawTaskData(SharedPtr<Allocator> allocator)
		: _data(nullptr, Deleter(allocator, 0)), _allocator(std::move(allocator)), _size(0)
	{

	}

	void RawTaskData::Allocate(Size size)
	{
		_size = size;
		_data = UniquePtr<unsigned char, Deleter>(
			(unsigned char*)_allocator->Allocate(size),
			Deleter(_allocator, size));
		std::memset(_data.get(), 0, size);
	}

	RawTaskData::RawTaskData(RawTaskData&& other) noexcept
		: _data(std::move(other._data)), _allocator(std::move(other._allocator))
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

		auto outputChunks = DataUtilities::SplitChunksToChunks<RawTaskData, RawTaskData>(inputChunks,
			{ _allocator },
			1,
			[](auto& allocator, auto chunkSize)
		{
			return std::make_unique<RawTaskData>(chunkSize, allocator);
		});

		*this = std::move(*outputChunks.front());
	}

	RawTaskData& RawTaskData::operator=(RawTaskData&& other) noexcept
	{
		_allocator = other._allocator;
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
	RawTaskData::Split(const Vector<SharedPtr<Allocator>>& targetAllocators) const
	{
		if (_size == 0)
		{
			Vector<UniquePtr<AbstractTaskData>> returnVector;
			returnVector.reserve(targetAllocators.size());

			for (auto& allocator: targetAllocators)
			{
				returnVector.push_back(std::make_unique<RawTaskData>(allocator));
			}
			return returnVector;
		}

		return DataUtilities::SplitChunksToChunks<RawTaskData, AbstractTaskData>({ *this }, targetAllocators, 1,
			[](auto& allocator, auto chunkSize)
			{
				return std::make_unique<RawTaskData>(chunkSize, allocator);
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

	RawTaskData::RawTaskData(Size size, SharedPtr<Allocator> allocator)
		: RawTaskData(std::move(allocator))
	{
		Allocate(size);
	}

	SharedPtr<Allocator> RawTaskData::GetAllocator() const
	{
		return _allocator;
	}
} // Elpida