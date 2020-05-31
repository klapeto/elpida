//
// Created by klapeto on 31/5/20.
//


#include "Benchmarks/Image/ConvertToUInt8/ConvertToUInt8Specification.hpp"
#include "Benchmarks/Image/ConvertToUInt8/ConvertToUInt8.hpp"
#include "Benchmarks/Image/ImageDataPropertiesTransformer.hpp"

namespace Elpida
{

	Task* ConvertToUInt8Specification::createNewTask(const TaskConfiguration& configuration,
		const ProcessorNode& processorToRun) const
	{
		return new ConvertToUInt8(*this, processorToRun);
	}

	ConvertToUInt8Specification::ConvertToUInt8Specification()
		: TaskSpecification("Convert to UInt8",
		ResultSpecification("Conversion Rate",
			"Pixels",
			ResultSpecification::Throughput,
			ResultSpecification::Accumulative,
			"The rate the pixels are converted from float to int"))
	{

		withDescription("Converts the image data from float to integer");
		withInputData(DataSpecification("Input image data",
			"Pixels",
			4,
			{ "width", "height" },
			"The input image data to convert to integer"));
		withOutputData(DataSpecification("Output image data",
			"Pixels",
			4,
			{ "width", "height" },
			"The output image data that were converted to integer"));

		withDataPropertiesTransformer(new ImageDataPropertiesTransformer());
	}
}