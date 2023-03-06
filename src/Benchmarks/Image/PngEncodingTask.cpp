//
// Created by klapeto on 1/3/2023.
//

#include "PngEncodingTask.hpp"
#include "Elpida/ElpidaException.hpp"

#include <png.h>

namespace Elpida
{
	void PngEncodingTask::Prepare(TaskData&& inputData)
	{
		_inputData = std::move(inputData);
		auto& metadata = _inputData->GetMetadata();

		auto str = metadata.at("width");
		_width = std::stoi(str);

		str = metadata.at("height");
		_height = std::stoi(str);

		_pngImg.width = _width;
		_pngImg.height = _height;

		std::size_t outputSize;

		if (png_image_write_to_memory(&_pngImg, nullptr, &outputSize, 0, _inputData->GetDataRaw(), 0, nullptr))
		{
			_outputData = std::make_unique<TaskData>(_inputData->GetTargetProcessor());
			_outputData->Allocate(outputSize);
		}
		else
		{
			throw ElpidaException("Failed to encode image: ", _pngImg.message);
		}
	}

	TaskData PngEncodingTask::Finalize()
	{
		return std::move(*_outputData);
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
			_outputData->GetDataRaw(),
			&outputSize,
			0,
			_inputData->GetDataRaw(),
			0,
			nullptr))
		{
			throw ElpidaException("Failed to encode image: ", _pngImg.message);
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