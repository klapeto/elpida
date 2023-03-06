//
// Created by klapeto on 5/3/2023.
//

#include "ConvertToGrayscaleTask.hpp"
#include "ImageUtilities.hpp"
#include "ImageBenchmarksConfig.hpp"

namespace Elpida
{
	void ConvertToGrayscaleTask::Prepare(TaskData&& inputData)
	{
		_inputData = std::move(inputData);

		auto& metadata = _inputData->GetMetadata();
		_width = std::stoul(metadata.at(WidthProperty));
		_height = std::stoul(metadata.at(HeightProperty));
		_channels = std::stoul(metadata.at(ChannelsProperty));
	}

	TaskData ConvertToGrayscaleTask::Finalize()
	{
		auto& metadata = _inputData->GetMetadata();

		metadata[WidthProperty] = std::to_string(_width);
		metadata[HeightProperty] = std::to_string(_height);
		metadata[ChannelsProperty] = std::to_string(_channels);
		metadata[BytesPerChannelProperty] = std::to_string(sizeof(FloatChannel));

		return std::move(*_inputData);
	}

	TaskInfo ConvertToGrayscaleTask::GetInfo() const
	{
		return TaskInfo("Convert to grayscale",
			"Converts RGBA pixels to float channels to grayscale",
			"Pixels",
			"The amount of pixels processed per second.",
			ScoreType::Throughput,
			DataInfo("Input image data", "The data of the image that is in RGBA float pixels.", "Pixels", {
				BytesPerChannelProperty, ChannelsProperty, WidthProperty, HeightProperty
			}),
			DataInfo("Output image data", "The data of the image converted to grayscale.", "Pixels", {
				BytesPerChannelProperty, ChannelsProperty, WidthProperty, HeightProperty
			}));
	}
	bool ConvertToGrayscaleTask::CanBeMultiThreaded() const
	{
		return false;
	}

	ConvertToGrayscaleTask::ConvertToGrayscaleTask()
		: _width(0), _height(0), _channels(0)
	{

	}

	void ConvertToGrayscaleTask::DoRun()
	{
		const std::size_t size = _width * _height * _channels;
		const auto channels = _channels;

		auto inPtr = (FloatChannel*)_inputData->GetDataRaw();

		for (std::size_t i = 0; i < size; i += channels)
		{
			FloatChannel valueToUse = (inPtr[i] + inPtr[i + 1] + inPtr[i + 2]) / (FloatChannel)3.0;
			inPtr[i] = valueToUse;
			inPtr[i + 1] = valueToUse;
			inPtr[i + 2] = valueToUse;
		}
	}
	std::unique_ptr<Task> ConvertToGrayscaleTask::DoDuplicate() const
	{
		return std::unique_ptr<Task>(new ConvertToGrayscaleTask());
	}
} // Elpida