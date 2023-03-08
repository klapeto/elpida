//
// Created by klapeto on 7/3/2023.
//

#include "ImageTaskData.hpp"
#include "Elpida/DataUtilities.hpp"

namespace Elpida
{
	Vector<UniquePtr<AbstractTaskData>>
	ImageTaskData::Split(const Vector<Ref<const ProcessingUnitNode>>& targetProcessors) const
	{
		auto stride = _width * _channels * _bytesPerChannel;

		return DataUtilities::SplitChunksToChunks<ImageTaskData, AbstractTaskData>({ *this },
			targetProcessors,
			stride,
			[this, stride](auto& processor, Size chunkSize)
			{
				return std::make_unique<ImageTaskData>(processor, _width,
					chunkSize / stride , _channels, _bytesPerChannel);
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

	ImageTaskData::ImageTaskData(const ProcessingUnitNode& targetProcessor, Size width, Size height, unsigned int channels, unsigned int bytesPerChannel)
		: RawTaskData(targetProcessor), _width(width), _height(height), _channels(channels), _bytesPerChannel(bytesPerChannel)
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