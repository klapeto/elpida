//
// Created by klapeto on 5/3/2023.
//

#include "ConvertToFloatTask.hpp"
#include "ImageBenchmarksConfig.hpp"
#include "Elpida/Core/ElpidaException.hpp"

namespace Elpida
{
	void ConvertToFloatTask::Prepare(SharedPtr<AbstractTaskData> inputData)
	{
		auto ptr = dynamic_cast<ImageTaskData*>(inputData.get());

		if (ptr->GetBytesPerChannel() != sizeof(IntChannel))
		{
			throw ElpidaException("ConvertToFloatTask requires image data in integer format");
		}

		_inputData = std::move(inputData);

		_outputData = std::make_shared<ImageTaskData>(
			ptr->GetAllocator(),
			ptr->GetWidth(),
			ptr->GetHeight(),
			ptr->GetChannels(),
			sizeof(FloatChannel));

		_channels = ptr->GetChannels();
		_sizeInChannels = ptr->GetWidth() * ptr->GetHeight() * _channels;
		_inPtr = static_cast<IntChannel*>(_inputData->GetData());
		_outPtr = reinterpret_cast<FloatChannel*>(_outputData->GetData());
	}

	SharedPtr<AbstractTaskData> ConvertToFloatTask::Finalize()
	{
		return std::move(_outputData);
	}

	TaskInfo ConvertToFloatTask::DoGetInfo() const
	{
		return { "Convert to float",
				 "Converts RGBA pixels to float channels (0.0 - 1.0)",
				 "Pixels",
				 "The amount of pixels processed per second.",
				 ScoreType::Throughput
		};
	}

	void ConvertToFloatTask::DoRun()
	{
		const auto divider = (FloatChannel)255.0;

		for (Size i = 0; i < _sizeInChannels; i += _channels)
		{
			_outPtr[i] = (FloatChannel)_inPtr[i] / divider;
			_outPtr[i + 1] = (FloatChannel)_inPtr[i + 1] / divider;
			_outPtr[i + 2] = (FloatChannel)_inPtr[i + 2] / divider;
			_outPtr[i + 3] = (FloatChannel)_inPtr[i + 3] / divider;
		}
	}

	UniquePtr<Task> ConvertToFloatTask::DoDuplicate() const
	{
		return UniquePtr<Task>(new ConvertToFloatTask());
	}

	ConvertToFloatTask::ConvertToFloatTask()
		: _inPtr(nullptr), _outPtr(nullptr), _sizeInChannels(0), _channels(0)
	{

	}

	Size ConvertToFloatTask::GetProcessedDataSize() const
	{
		return _outputData->GetWidth() * _outputData->GetHeight();
	}
} // Elpida