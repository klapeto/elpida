//
// Created by klapeto on 5/3/2023.
//

#include "ConvertToUInt8Task.hpp"

#include "ImageBenchmarksConfig.hpp"
#include "ImageTaskData.hpp"
#include "Elpida/Core/ElpidaException.hpp"

#include <cstdint>

namespace Elpida
{
	void ConvertToUInt8Task::Prepare(UniquePtr<AbstractTaskData> inputData)
	{
		auto ptr = dynamic_cast<ImageTaskData*>(inputData.get());

		if (ptr->GetBytesPerChannel() != sizeof(FloatChannel))
		{
			throw ElpidaException("ConvertToUInt8Task requires image data in float format");
		}

		_inputData = std::move(inputData);

		_outputData = std::make_unique<ImageTaskData>(
			ptr->GetAllocator(),
			ptr->GetWidth(),
			ptr->GetHeight(),
			ptr->GetChannels(),
			sizeof(IntChannel));

		_channels = ptr->GetChannels();
		_sizeInChannels = ptr->GetWidth() * ptr->GetHeight() * _channels;
		_inPtr = reinterpret_cast<FloatChannel*>(_inputData->GetData());
		_outPtr = static_cast<IntChannel*>(_outputData->GetData());
	}

	UniquePtr<AbstractTaskData> ConvertToUInt8Task::Finalize()
	{
		return std::move(_outputData);
	}

	TaskInfo ConvertToUInt8Task::GetInfo() const
	{
		return {
			"Convert to UInt8",
			"Converts RGBA float pixels to uint8 channels (0 - 255)",
			"Pixels",
			"The amount of pixels processed per second.",
			ScoreType::Throughput
		};
	}
	bool ConvertToUInt8Task::CanBeMultiThreaded() const
	{
		return true;
	}

	ConvertToUInt8Task::ConvertToUInt8Task()
		: _outPtr(nullptr), _inPtr(nullptr), _sizeInChannels(0), _channels(0)
	{

	}

	void ConvertToUInt8Task::DoRun()
	{
		const auto multiplier = (FloatChannel)255.0;

		for (std::size_t i = 0; i < _sizeInChannels; i += _channels)
		{
			_outPtr[i] = (IntChannel)(_inPtr[i] * multiplier);
			_outPtr[i + 1] = (IntChannel)(_inPtr[i + 1] * multiplier);
			_outPtr[i + 2] = (IntChannel)(_inPtr[i + 2] * multiplier);
			_outPtr[i + 3] = (IntChannel)(_inPtr[i + 3] * multiplier);
		}
	}

	UniquePtr<Task> ConvertToUInt8Task::DoDuplicate() const
	{
		return UniquePtr<Task>(new ConvertToUInt8Task());
	}

	Size ConvertToUInt8Task::GetProcessedDataSize() const
	{
		return _sizeInChannels / _channels;
	}
} // Elpida