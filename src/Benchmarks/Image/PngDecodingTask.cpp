//
// Created by klapeto on 2/3/2023.
//

#include "PngDecodingTask.hpp"

#include <png.h>

namespace Elpida
{
	void PngDecodingTask::Prepare(TaskData&& inputData)
	{
		if (inputData.GetSize() == 0)
		{
			// throw;
		}

		_inputData = std::move(inputData);

		if (png_image_begin_read_from_memory(&_pngImg, _inputData.GetDataRaw(), _inputData.GetSize()))
		{
			_pngImg.format = PNG_FORMAT_RGBA;

			auto newSize = PNG_IMAGE_SIZE(_pngImg);

			_outputData.Migrate(_inputData.GetProcessorId());
			_outputData.Allocate(newSize);
		}
		else
		{
			std::string error(_pngImg.message);
			//throw ElpidaException(FUNCTION_NAME, Vu::Cs("Failed to encode image: ", error));
		}
	}

	TaskData PngDecodingTask::Finalize()
	{
		auto metadata = _outputData.GetMetadata();
		metadata["width"] = std::to_string(_pngImg.width);
		metadata["height"] = std::to_string(_pngImg.height);
		metadata["stride"] = std::to_string(_pngImg.width * 4);
		return std::move(_outputData);
	}

	TaskInfo PngDecodingTask::GetInfo() const
	{
		return TaskInfo(
			"PNG decoding",
			"Decodes a stream of data from PNG decoded data to RGBA data.",
			"B",
			"The input amount of bytes processed per second",
			ScoreType::Throughput,
			Elpida::DataInfo(),
			DataInfo("Output image data", "The data of the image to encode to RGBA 8bpp format.", "Pixels", {
				"stride", "width", "height"
			}));
	}

	bool PngDecodingTask::CanBeMultiThreaded() const
	{
		return false;
	}

	void PngDecodingTask::DoRun()
	{
		if (!png_image_finish_read(&_pngImg, nullptr, _inputData.GetDataRaw(), 0, nullptr))
		{
			std::string error(_pngImg.message);
			//throw ElpidaException(FUNCTION_NAME, Vu::Cs("Failed to encode image: ", error));
		}
	}
	std::unique_ptr<Task> PngDecodingTask::DoDuplicate() const
	{
		return std::unique_ptr<Task>(new PngDecodingTask());
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