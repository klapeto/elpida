//
// Created by klapeto on 5/3/2023.
//

#include "ConvertToUInt8Task.hpp"
#include "ImageUtilities.hpp"

#include "ImageBenchmarksConfig.hpp"

#include <cstdint>

namespace Elpida
{
	void ConvertToUInt8Task::Prepare(TaskData&& inputData)
	{
		_inputData = std::move(inputData);

		auto& metadata = _inputData->GetMetadata();
		_width = std::stoul(metadata.at(WidthProperty));
		_height = std::stoul(metadata.at(HeightProperty));
		_inputBytesPerChannel = std::stoul(metadata.at(BytesPerChannelProperty));
		_channels = std::stoul(metadata.at(ChannelsProperty));

		_outputData = std::make_unique<TaskData>(_inputData->GetTargetProcessor());
		_outputData->Allocate(_width * _height * _channels * sizeof(IntChannel));
	}

	TaskData ConvertToUInt8Task::Finalize()
	{
		auto& metadata = _outputData->GetMetadata();
		metadata[WidthProperty] = std::to_string(_width);
		metadata[HeightProperty] = std::to_string(_height);
		metadata[ChannelsProperty] = std::to_string(_channels);
		metadata[BytesPerChannelProperty] = std::to_string(sizeof(IntChannel));

		return std::move(*_outputData);
	}

	TaskInfo ConvertToUInt8Task::GetInfo() const
	{
		return TaskInfo("Convert to UInt8",
			"Converts RGBA float pixels to uint8 channels (0 - 255)",
			"Pixels",
			"The amount of pixels processed per second.",
			ScoreType::Throughput,
			DataInfo("Input image data", "The data of the image that is in RGBA float pixels.", "Pixels", {
				BytesPerChannelProperty, ChannelsProperty, WidthProperty, HeightProperty
			}),
			DataInfo("Output image data", "The data of the image converted to uint8 RGBA channels with value 0 - 255", "Pixels", {
				BytesPerChannelProperty, ChannelsProperty, WidthProperty, HeightProperty
			}));
	}
	bool ConvertToUInt8Task::CanBeMultiThreaded() const
	{
		return false;
	}

	ConvertToUInt8Task::ConvertToUInt8Task()
		: _width(0), _height(0), _channels(0), _inputBytesPerChannel(0)
	{

	}

	void ConvertToUInt8Task::DoRun()
	{
		const std::size_t size = _width * _height * _channels;
		const auto multiplier = (FloatChannel)255.0;
		const auto channels = _channels;

		auto outPtr = (IntChannel*)_outputData->GetDataRaw();
		auto inPtr = (FloatChannel *)_inputData->GetDataRaw();

		for (std::size_t i = 0; i < size; i += channels)
		{
			outPtr[i] = (IntChannel)(inPtr[i] * multiplier);
			outPtr[i + 1] = (IntChannel)(inPtr[i + 1] * multiplier);
			outPtr[i + 2] = (IntChannel)(inPtr[i + 2] * multiplier);
			outPtr[i + 3] = (IntChannel)(inPtr[i + 3] * multiplier);
		}
	}

	std::unique_ptr<Task> ConvertToUInt8Task::DoDuplicate() const
	{
		return std::unique_ptr<Task>(new ConvertToUInt8Task());
	}
} // Elpida