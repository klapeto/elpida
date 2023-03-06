//
// Created by klapeto on 5/3/2023.
//

#include "ImageUtilities.hpp"
#include "ImageBenchmarksConfig.hpp"

#include <string>
#include <cmath>

namespace Elpida
{
	void ImageUtilities::TransformMetadataForChunk(std::size_t originalSize, TaskData::Metadata& metadata, std::size_t targetSize)
	{
		auto width = std::stoul(metadata.at(WidthProperty));
		auto bytesPerChannel = std::stoul(metadata.at(BytesPerChannelProperty));
		auto channels = std::stoul(metadata.at(ChannelsProperty));

		auto targetWidth = width;
		auto targetStride = width * bytesPerChannel * channels;
		auto targetHeight = std::round(targetSize / targetStride);

		metadata[WidthProperty] = std::to_string(targetWidth);
		metadata[HeightProperty] = std::to_string((std::size_t)targetHeight);
	}

	std::size_t ImageUtilities::GetChunkDivisibleBy(const TaskData& taskData)
	{
		auto metadata = taskData.GetMetadata();
		auto width = std::stoul(metadata.at(WidthProperty));
		auto bytesPerChannel = std::stoul(metadata.at(BytesPerChannelProperty));
		auto channels = std::stoul(metadata.at(ChannelsProperty));
		return width * bytesPerChannel * channels;
	}
} // Elpida