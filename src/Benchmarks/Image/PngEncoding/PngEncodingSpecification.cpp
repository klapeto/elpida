//
// Created by klapeto on 31/5/20.
//

#include "Benchmarks/Image/PngEncoding/PngEncoding.hpp"
#include "Benchmarks/Image/PngEncoding/PngEncodingSpecification.hpp"

namespace Elpida
{
	Task* PngEncodingSpecification::createNewTask(const TaskConfiguration& configuration,
		const ProcessorNode& processorToRun) const
	{
		return new PngEncoding(*this, processorToRun);
	}

	PngEncodingSpecification::PngEncodingSpecification()
		: TaskSpecification("Encode to PNG",
		ResultSpecification("Encode rate",
			"Pixels",
			ResultSpecification::Throughput,
			ResultSpecification::Accumulative,
			"The rate that pixels are encoded into PNG format"))
	{

		withDescription("Encodes the image data to PNG Format");
		withInputData(DataSpecification("Input image data",
			"Pixels",
			4,
			{ "width", "height" },
			"The input image data in UInt8 to encode into PNG"));
		withOutputData(DataSpecification("PNG data", "B", "The image encoded to PNG format"));
	}
}
