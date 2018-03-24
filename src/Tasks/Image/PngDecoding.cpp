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
