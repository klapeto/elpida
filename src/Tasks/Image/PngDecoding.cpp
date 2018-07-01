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
 * PngLoad.cpp
 *
 *  Created on: 18 Μαρ 2018
 *      Author: klapeto
 */

#include "Tasks/Image/PngDecoding.hpp"

#include "Tasks/Image/Encoders/LibPngEncoder.hpp"

namespace Elpida
{

	PngDecoding::PngDecoding(unsigned char* const& inputData, const size_t& dataSize) :
					Task("Png Decoding"), _inputData(inputData), _dataSize(dataSize)
	{

	}

	PngDecoding::~PngDecoding()
	{

	}

	void PngDecoding::run()
	{
		LibPngEncoder encoder;
		auto decodeInfo = encoder.decode((unsigned char*)_inputData, (size_t)_dataSize);
		_image.setData(decodeInfo.data, decodeInfo.width, decodeInfo.height, true);
	}

	TaskThroughput PngDecoding::translateToThroutput(const TaskMetrics& metrics) const
	{
		return TaskThroughput(TaskThroughput::getValueScale(((double)_dataSize) / metrics.getSeconds()) + "B/s");
	}

} /* namespace Elpida */
