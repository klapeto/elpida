//
// Created by klapeto on 1/3/2023.
//

#include "PngEncodingTask.hpp"

#include <png.h>

namespace Elpida
{
	void PngEncodingTask::Prepare(TaskData&& inputData)
	{
		auto& metadata = inputData.GetMetadata();

		auto str = metadata.at("width");
		char* endPtr;
		_width = std::strtol(str.c_str(), &endPtr, 0);

		str = metadata.at("height");
		_height = std::strtol(str.c_str(), &endPtr, 0);

		_inputData = std::move(inputData);

		_pngImg.width = _width;
		_pngImg.height = _height;

		std::size_t outputSize;

		if (png_image_write_to_memory(&_pngImg, nullptr, &outputSize, 0, _inputData.GetDataRaw(), 0, nullptr))
		{
			_outputData.Migrate(_inputData.GetProcessorId());
			_outputData.Allocate(outputSize);

		}
		else
		{
			std::string error(_pngImg.message);
			//throw ElpidaException(FUNCTION_NAME, Vu::Cs("Failed to encode image: ", error));
		}
	}

	TaskData PngEncodingTask::Finalize()
	{
		return std::move(_outputData);
	}

	TaskInfo PngEncodingTask::GetInfo() const
	{
		return TaskInfo("Png encoding",
			"Encodes data to png format.",
			"Pixels",
			"The amount of pixels encoded.",
			ScoreType::Throughput,
			DataInfo("Input image data", "The data of the image to encode to RGBA 8bpp format.", "Pixels", {
				"stride", "width", "height"
			}),
			Elpida::DataInfo());
	}

	bool PngEncodingTask::CanBeMultiThreaded() const
	{
		return false;
	}

	void PngEncodingTask::DoRun()
	{
		std::size_t outputSize;

		if (!png_image_write_to_memory(&_pngImg,
			_outputData.GetDataRaw(),
			&outputSize,
			0,
			_inputData.GetDataRaw(),
			0,
			nullptr))
		{
			std::string error(_pngImg.message);
			//throw ElpidaException(FUNCTION_NAME, Vu::Cs("Failed to encode image: ", error));
		}
	}

	std::unique_ptr<Task> PngEncodingTask::DoDuplicate() const
	{
		return std::unique_ptr<Task>(new PngEncodingTask());
	}
	PngEncodingTask::~PngEncodingTask()
	{

		png_image_free(&_pngImg);
	}
	PngEncodingTask::PngEncodingTask()
		: _pngImg({}), _width(0), _height(0)
	{
		_pngImg.opaque = nullptr;
		_pngImg.version = PNG_IMAGE_VERSION;
		_pngImg.format = PNG_FORMAT_RGBA;
	}
} // Elpida