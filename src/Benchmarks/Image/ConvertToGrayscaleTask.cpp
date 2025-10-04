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
// Created by klapeto on 5/3/2023.
//

#include "ConvertToGrayscaleTask.hpp"

#include "ImageBenchmarksConfig.hpp"
#include "Elpida/Core/ElpidaException.hpp"
#include "ImageTaskData.hpp"

namespace Elpida
{
	void ConvertToGrayscaleTask::Prepare(SharedPtr<AbstractTaskData> inputData)
	{
		auto ptr = dynamic_cast<ImageTaskData*>(inputData.get());

		if (ptr->GetBytesPerChannel() != sizeof(FloatChannel))
		{
			throw ElpidaException("ConvertToFloatTask requires image data in integer format");
		}

		_inputData = std::move(inputData);

		_channels = ptr->GetChannels();
		_sizeInChannels = ptr->GetWidth() * ptr->GetHeight() * _channels;

		_inPtr = reinterpret_cast<FloatChannel*>(_inputData->GetData());
	}

	SharedPtr<AbstractTaskData> ConvertToGrayscaleTask::Finalize()
	{
		return std::move(_inputData);
	}

	TaskInfo ConvertToGrayscaleTask::DoGetInfo() const
	{
		return {
				"Convert to grayscale",
				"Converts RGBA pixels to float channels to grayscale",
				"Pixels",
				"The amount of pixels processed per second.",
				ResultType::Throughput
		};
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

	Size ConvertToGrayscaleTask::GetProcessedDataSize() const
	{
		return _sizeInChannels / _channels;
	}
} // Elpida