//
// Created by klapeto on 7/3/2023.
//

#include "ImageTaskData.hpp"
#include "Elpida/Core/DataUtilities.hpp"

namespace Elpida
{
	Vector<UniquePtr<AbstractTaskData>>
	ImageTaskData::Split(const Vector<SharedPtr<Allocator>>& targetAllocators) const
	{
		auto stride = _width * _channels * _bytesPerChannel;

		return DataUtilities::SplitChunksToChunks<ImageTaskData, AbstractTaskData>({ *this },
			targetAllocators,
			stride,
			[this, stride](auto& allocator, Size chunkSize)
			{
				return std::make_unique<ImageTaskData>(allocator,
					_width,
					chunkSize / stride ,
					_channels,
					_bytesPerChannel);
			});
	}

	Size ImageTaskData::GetWidth() const
	{
		return _width;
	}

	Size ImageTaskData::GetHeight() const
	{
		return _height;
	}

	unsigned int ImageTaskData::GetChannels() const
	{
		return _channels;
	}

	unsigned int ImageTaskData::GetBytesPerChannel() const
	{
		return _bytesPerChannel;
	}

	ImageTaskData::ImageTaskData(SharedPtr<Allocator> allocator, Size width, Size height, unsigned int channels, unsigned int bytesPerChannel)
		: RawTaskData(std::move(allocator)), _width(width), _height(height), _channels(channels), _bytesPerChannel(bytesPerChannel)
	{
		RawTaskData::Allocate(_width * _height * _channels * _bytesPerChannel);
	}

	void ImageTaskData::Merge(const Vector<UniquePtr<AbstractTaskData>>& data)
	{
		Size totalHeight = 0;
		for (auto& chunk: data)
		{
			auto imageData = dynamic_cast<ImageTaskData*>(chunk.get());
			totalHeight += imageData->GetHeight();
		}

		auto first = static_cast<ImageTaskData*>(data.front().get());
		_width = first->GetWidth();
		_height = totalHeight;
		_channels = first->GetChannels();
		_bytesPerChannel = first->GetBytesPerChannel();

		RawTaskData::Merge(data);
	}
} // Elpida