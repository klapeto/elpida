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

#include "TaskBatches/Image/PngDecoding.hpp"

#include "Elpida/TaskMetrics.hpp"
#include "Elpida/Utilities/ImageEncoder.hpp"
#include "TaskBatches/Image/Encoders/LibPngEncoder.hpp"

namespace Elpida
{

	PngDecoding::PngDecoding(const DataPtr& inputData, const std::size_t& dataSize)
			: Task("Png Decoding"), _runResult("Data process rate", "Bytes"), _inputData(inputData), _dataSize(dataSize)
	{

	}

	PngDecoding::~PngDecoding()
	{

	}

	void PngDecoding::run()
	{
		LibPngEncoder encoder;
		auto decodeInfo = encoder.decode((DataPtr) _inputData, (std::size_t) _dataSize);
		_image.setData(decodeInfo.data, decodeInfo.width, decodeInfo.height, true);
	}

	void PngDecoding::calculateResults(const TaskMetrics& metrics)
	{
		_runResult.setOriginalValue(_dataSize);
		addResult(_runResult);
	}

} /* namespace Elpida */
