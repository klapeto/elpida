//
// Created by klapeto on 1/3/2023.
//

#include "PngEncodingTask.hpp"
#include "Elpida/Core/ElpidaException.hpp"
#include "ImageTaskData.hpp"

#include <png.h>

namespace Elpida
{
	void PngEncodingTask::Prepare(SharedPtr<AbstractTaskData> inputData)
	{
		auto ptr = dynamic_cast<ImageTaskData*>(inputData.get());

		if (inputData->GetSize() == 0)
		{
			throw ElpidaException("Png decoding must be supplied with data. It got zero size data.");
		}

		_inputData = std::move(inputData);

		std::size_t outputSize;

		_pngImg.width = ptr->GetWidth();
		_pngImg.height = ptr->GetHeight();

		if (png_image_write_to_memory(&_pngImg, nullptr, &outputSize, 0, _inputData->GetData(), 0, nullptr))
		{
			_outputData = std::make_shared<RawTaskData>(_inputData->GetAllocator());
			_outputData->Allocate(outputSize);
		}
		else
		{
			throw ElpidaException("Failed to encode image: ", _pngImg.message);
		}
	}

	SharedPtr<AbstractTaskData> PngEncodingTask::Finalize()
	{
		return std::move(_outputData);
	}

	TaskInfo PngEncodingTask::DoGetInfo() const
	{
		return {
				"Png encoding",
				"Encodes data to png format.",
				"Pixels",
				"The amount of pixels encoded.",
				ResultType::Throughput
		};
	}

	void PngEncodingTask::DoRun()
	{
		std::size_t outputSize = _outputData->GetSize();

		if (!png_image_write_to_memory(&_pngImg,
			_outputData->GetData(),
			&outputSize,
			0,
			_inputData->GetData(),
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

	Size PngEncodingTask::GetProcessedDataSize() const
	{
		return _pngImg.width * _pngImg.height;
	}
} // Elpida