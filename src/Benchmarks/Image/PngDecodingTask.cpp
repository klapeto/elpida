/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

//
// Created by klapeto on 2/3/2023.
//

#include "PngDecodingTask.hpp"
#include "Elpida/Core/ElpidaException.hpp"
#include "ImageBenchmarksConfig.hpp"

#include <png.h>

namespace Elpida
{
	void PngDecodingTask::Prepare(SharedPtr<AbstractTaskData> inputData)
	{
		if (inputData->GetSize() == 0)
		{
			throw ElpidaException("Png decoding must be supplied with data. It got zero size data.");
		}

		_inputData = std::move(inputData);

		if (png_image_begin_read_from_memory(&_pngImg, _inputData->GetData(), _inputData->GetSize()) != 0)
		{
			_pngImg.format = PNG_FORMAT_RGBA;

			_outputData = std::make_unique<ImageTaskData>(_inputData->GetAllocator(), _pngImg.width, _pngImg.height, 4, sizeof(IntChannel));
			_outputData->Allocate(PNG_IMAGE_SIZE(_pngImg));
		}
		else
		{
			throw ElpidaException("Failed to decode image: ", _pngImg.message);
		}
	}

	SharedPtr<AbstractTaskData> PngDecodingTask::Finalize()
	{
		return std::move(_outputData);
	}

	TaskInfo PngDecodingTask::DoGetInfo() const
	{
		return {
				"PNG decoding",
				"Decodes a stream of data from PNG decoded data to RGBA data.",
				"B",
				"The input amount of bytes processed per second",
				ResultType::Throughput
		};
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

	Size PngDecodingTask::GetProcessedDataSize() const
	{
		return _inputData->GetSize();
	}
} // Elpida