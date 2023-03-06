//
// Created by klapeto on 5/3/2023.
//

#include "ConvertToFloatTask.hpp"
#include "Elpida/ElpidaException.hpp"
#include "ImageUtilities.hpp"
#include "ImageBenchmarksConfig.hpp"

namespace Elpida
{
	void ConvertToFloatTask::Prepare(TaskData&& inputData)
	{
		_inputData = std::move(inputData);

		auto& metadata = _inputData->GetMetadata();
		_width = std::stoul(metadata.at(WidthProperty));
		_height = std::stoul(metadata.at(HeightProperty));
		_inputBytesPerChannel = std::stoul(metadata.at(BytesPerChannelProperty));
		_channels = std::stoul(metadata.at(ChannelsProperty));

		_outputData = std::make_unique<TaskData>(_inputData->GetTargetProcessor());
		_outputData->Allocate(_width * _height * _channels * sizeof(FloatChannel));
	}

	TaskData ConvertToFloatTask::Finalize()
	{
		auto& metadata = _outputData->GetMetadata();

		metadata[WidthProperty] = std::to_string(_width);
		metadata[HeightProperty] = std::to_string(_height);
		metadata[ChannelsProperty] = std::to_string(_channels);
		metadata[BytesPerChannelProperty] = std::to_string(sizeof(FloatChannel));

		return std::move(*_outputData);
	}

	TaskInfo ConvertToFloatTask::GetInfo() const
	{
		return TaskInfo("Convert to float",
			"Converts RGBA pixels to float channels (0.0 - 1.0)",
			"Pixels",
			"The amount of pixels processed per second.",
			ScoreType::Throughput,
			DataInfo("Input image data", "The data of the image that is in RGBA 32bpp pixels.", "Pixels", {
				BytesPerChannelProperty, ChannelsProperty, WidthProperty, HeightProperty
			}),
			DataInfo("Output image data", "The data of the image converted to float RGBA channels with value 0.0 - 1.0", "Pixels", {
				BytesPerChannelProperty, ChannelsProperty, WidthProperty, HeightProperty
			}));
	}

	bool ConvertToFloatTask::CanBeMultiThreaded() const
	{
		return false;
	}

	void ConvertToFloatTask::DoRun()
	{
		const std::size_t size = _width * _height * _channels;
		const auto divider = (FloatChannel)255.0;

		auto outPtr = (FloatChannel*)_outputData->GetDataRaw();
		auto inPtr = (IntChannel*)_inputData->GetDataRaw();

		for (std::size_t i = 0; i < size; i += _channels)
		{
			outPtr[i] = (FloatChannel)inPtr[i] / divider;
			outPtr[i + 1] = (FloatChannel)inPtr[i + 1] / divider;
			outPtr[i + 2] = (FloatChannel)inPtr[i + 2] / divider;
			outPtr[i + 3] = (FloatChannel)inPtr[i + 3] / divider;
		}
	}

	std::unique_ptr<Task> ConvertToFloatTask::DoDuplicate() const
	{
		return std::unique_ptr<Task>(new ConvertToFloatTask());
	}

	ConvertToFloatTask::ConvertToFloatTask()
		: _width(0), _height(0), _channels(0), _inputBytesPerChannel(0)
	{

	}
} // Elpida