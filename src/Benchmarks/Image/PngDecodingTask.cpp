//
// Created by klapeto on 2/3/2023.
//

#include "PngDecodingTask.hpp"
#include "Elpida/ElpidaException.hpp"
#include "ImageBenchmarksConfig.hpp"

#include <png.h>

namespace Elpida
{
	void PngDecodingTask::Prepare(UniquePtr<AbstractTaskData> inputData)
	{
		if (inputData->GetSize() == 0)
		{
			throw ElpidaException("Png decoding must be supplied with data. It got zero size data.");
		}

		_inputData = std::move(inputData);

		if (png_image_begin_read_from_memory(&_pngImg, _inputData->GetData(), _inputData->GetSize()) != 0)
		{
			_pngImg.format = PNG_FORMAT_RGBA;

			_outputData = std::make_unique<ImageTaskData>(_inputData->GetTargetProcessor(), _pngImg.width, _pngImg.height, 4, sizeof(IntChannel));
			_outputData->Allocate(PNG_IMAGE_SIZE(_pngImg));
		}
		else
		{
			throw ElpidaException("Failed to decode image: ", _pngImg.message);
		}
	}

	UniquePtr<AbstractTaskData> PngDecodingTask::Finalize()
	{
		return std::move(_outputData);
	}

	TaskInfo PngDecodingTask::GetInfo() const
	{
		return {
			"PNG decoding",
			"Decodes a stream of data from PNG decoded data to RGBA data.",
			"B",
			"The input amount of bytes processed per second",
			ScoreType::Throughput
		};
	}

	bool PngDecodingTask::CanBeMultiThreaded() const
	{
		return false;
	}

	void PngDecodingTask::DoRun()
	{
		if (png_image_finish_read(&_pngImg, nullptr, _outputData->GetData(), 0, nullptr) == 0)
		{
			throw ElpidaException("Failed to decode image: ", _pngImg.message);
		}
	}
	UniquePtr<Task> PngDecodingTask::DoDuplicate() const
	{
		return UniquePtr<Task>(new PngDecodingTask());
	}

	PngDecodingTask::~PngDecodingTask()
	{
		png_image_free(&_pngImg);
	}

	PngDecodingTask::PngDecodingTask()
		: _pngImg({})
	{
		_pngImg.opaque = nullptr;
		_pngImg.version = PNG_IMAGE_VERSION;
	}
} // Elpida