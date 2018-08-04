/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2018  Ioannis Panagiotopoulos
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>
 *************************************************************************/

/*
 * PngWrite.cpp
 *
 *  Created on: 18 Μαρ 2018
 *      Author: klapeto
 */

#include "PngEncoding.hpp"

#include "Elpida/TaskMetrics.hpp"
#include "Elpida/Types/Float.hpp"
#include "Encoders/LibPngEncoder.hpp"

namespace Elpida
{

	PngEncoding::PngEncoding(const Image<RawData>& inputImage)
			: Task("Png Encoding"), _inputImage(inputImage), _encodedData(nullptr), _encodedDataSize(0)
	{
	}

	PngEncoding::~PngEncoding()
	{
		if (_encodedData != nullptr)
		{
			delete[] _encodedData;
		}
	}

	void PngEncoding::run()
	{
		LibPngEncoder encoder;
		auto encodeResult = encoder.encode(_inputImage.getWidth(), _inputImage.getHeight(), (RawDataPtr) _inputImage.getData(),
		                                   _inputImage.getTotalSizeInBytes());
		_encodedData = encodeResult.data;
		_encodedDataSize = encodeResult.dataSize;
	}

	TaskThroughput PngEncoding::translateToThroutput(const TaskMetrics& metrics) const
	{
		return TaskThroughput(TaskThroughput::getValueScale(((Float64) _inputImage.getTotalSize() * 4) / metrics.getSeconds()) + "B/s");
	}

} /* namespace Elpida */
