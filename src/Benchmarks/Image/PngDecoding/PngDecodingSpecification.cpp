//
// Created by klapeto on 31/5/20.
//

#include "Benchmarks/Image/PngDecoding/PngDecodingSpecification.hpp"

#include "Benchmarks/Image/PngDecoding/PngDecoding.hpp"

namespace Elpida
{

	Task* PngDecodingSpecification::createNewTask(const TaskConfiguration& configuration,
		const ProcessorNode& processorToRun) const
	{
		return new PngDecoding(*this, processorToRun);
	}

	PngDecodingSpecification::PngDecodingSpecification()
		: TaskSpecification("Png Decoding",
		ResultSpecification("Decode Rate", "B", ResultSpecification::Throughput, ResultSpecification::Accumulative))
	{
		withDescription("Decodes raw Png data to RGBA pixel array");
		withInputData(DataSpecification("Input file data", "B", "The raw png file data"));
		withOutputData(DataSpecification("Output image data",
			"Pixels",
			4,
			{ "width", "height" },
			"The image data in RGBA pixel array"));
	}
}