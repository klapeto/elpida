/*
 * PngWrite.cpp
 *
 *  Created on: 18 Μαρ 2018
 *      Author: klapeto
 */

#include "Tasks/Image/PngEncoding.hpp"

#include "Tasks/Image/Encoders/LibPngEncoder.hpp"

namespace Elpida
{

	PngEncoding::PngEncoding(const Image<unsigned char>& inputImage) :
			Task("Png Encoding"),
			_inputImage(inputImage),
			_encodedData(nullptr),
			_encodedDataSize(0)
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
		auto encodeResult = encoder.encode(_inputImage.getWidth(), _inputImage.getHeight(),
				(unsigned char*) _inputImage.getData(), _inputImage.getTotalSizeInBytes());
		_encodedData = encodeResult.data;
		_encodedDataSize = encodeResult.dataSize;
	}

	TaskThroughput PngEncoding::translateToThroutput(const TaskMetrics& metrics) const
	{
		return TaskThroughput(
				TaskThroughput::getValueScale(((double) _inputImage.getTotalSize() * 4) / metrics.getSeconds()) + "B/s");
	}

} /* namespace Elpida */
