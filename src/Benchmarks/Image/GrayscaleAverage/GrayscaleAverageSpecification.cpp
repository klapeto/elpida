//
// Created by klapeto on 31/5/20.
//

#include "Benchmarks/Image/GrayscaleAverage/GrayscaleAverageSpecification.hpp"
#include "Benchmarks/Image/ImageDataPropertiesTransformer.hpp"
#include "Benchmarks/Image/GrayscaleAverage/GrayscaleAverage.hpp"

namespace Elpida
{

	Task* GrayscaleAverageSpecification::createNewTask(const TaskConfiguration& configuration,
		const ProcessorNode& processorToRun) const
	{
		return new GrayscaleAverage(*this, processorToRun);
	}

	GrayscaleAverageSpecification::GrayscaleAverageSpecification()
		: TaskSpecification("Convert to Gray Scale (Average)",
		ResultSpecification("Conversion Rate",
			"Pixels",
			ResultSpecification::Throughput,
			ResultSpecification::Accumulative,
			"Pixel conversion rate"))
	{
		withDescription("Converts image data to Gray scale using the average of pixels");

		withInputData(DataSpecification("Input Image data",
			"Pixels",
			4,
			{ "width", "height" },
			"The image to convert to grayscale in float pixels"));
		withOutputData(DataSpecification("Output Image data",
			"Pixels",
			4,
			{ "width", "height" },
			"The converted image in float pixels"));

		withDataPropertiesTransformer(new ImageDataPropertiesTransformer());
	}
}