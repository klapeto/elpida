//
// Created by klapeto on 31/5/20.
//


#include "Benchmarks/Image/ConvertToFloat/ConvertToFloatSpecification.hpp"
#include "Benchmarks/Image/ConvertToFloat/ConvertToFloat.hpp"
#include "Benchmarks/Image/ImageDataPropertiesTransformer.hpp"

namespace Elpida
{

	Task* ConvertToFloatSpecification::createNewTask(const TaskConfiguration& configuration,
		const ProcessorNode& processorToRun) const
	{
		return new ConvertToFloat(*this, processorToRun);
	}

	ConvertToFloatSpecification::ConvertToFloatSpecification()
		: TaskSpecification("Convert To Float",
		ResultSpecification("Convert Rate",
			"Pixels",
			ResultSpecification::Type::Throughput,
			ResultSpecification::AggregationType::Accumulative))
	{

		withDescription("Converts an Image with integer values to float values between 0.0 and 1.0");

		withInputData(DataSpecification("Input image",
			"Pixels",
			4,
			{ "width", "height" },
			"Input image in RGBA and integer values (0-255)"));

		withOutputData(DataSpecification("Output image",
			"Pixels",
			4,
			{ "width", "height" },
			"Output image in RGBA and float values (0.0-1.0)"));

		withDataPropertiesTransformer(new ImageDataPropertiesTransformer());
	}
}