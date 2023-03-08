//
// Created by klapeto on 5/3/2023.
//

#include "ConvertToGrayscaleTask.hpp"

#include "ImageBenchmarksConfig.hpp"
#include "Elpida/ElpidaException.hpp"
#include "ImageTaskData.hpp"

namespace Elpida
{
	void ConvertToGrayscaleTask::Prepare(UniquePtr<AbstractTaskData> inputData)
	{
		auto ptr = dynamic_cast<ImageTaskData*>(inputData.get());

		if (ptr->GetBytesPerChannel() != sizeof(FloatChannel))
		{
			throw ElpidaException("ConvertToFloatTask requires image data in integer format");
		}

		_inputData = std::move(inputData);

		_channels = ptr->GetChannels();
		_sizeInChannels = ptr->GetWidth() * ptr->GetHeight() * _channels;

		_inPtr = reinterpret_cast<FloatChannel*>(_inputData->GetDataRaw());
	}

	UniquePtr<AbstractTaskData> ConvertToGrayscaleTask::Finalize()
	{
		return std::move(_inputData);
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
		return true;
	}

	ConvertToGrayscaleTask::ConvertToGrayscaleTask()
		: _inPtr(nullptr), _sizeInChannels(0), _channels(0)
	{

	}

	void ConvertToGrayscaleTask::DoRun()
	{
		for (std::size_t i = 0; i < _sizeInChannels; i += _channels)
		{
			FloatChannel valueToUse = (_inPtr[i] + _inPtr[i + 1] + _inPtr[i + 2]) / (FloatChannel)3.0;
			_inPtr[i] = valueToUse;
			_inPtr[i + 1] = valueToUse;
			_inPtr[i + 2] = valueToUse;
		}
	}

	UniquePtr<Task> ConvertToGrayscaleTask::DoDuplicate() const
	{
		return UniquePtr<Task>(new ConvertToGrayscaleTask());
	}
} // Elpida