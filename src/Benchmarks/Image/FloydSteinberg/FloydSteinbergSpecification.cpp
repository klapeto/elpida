//
// Created by klapeto on 31/5/20.
//

#include "Benchmarks/Image/FloydSteinberg/FloydSteinbergSpecification.hpp"
#include "Benchmarks/Image/FloydSteinberg/FloydSteinberg.hpp"

namespace Elpida
{

	Task* FloydSteinbergSpecification::createNewTask(const TaskConfiguration& configuration,
		const ProcessorNode& processorToRun) const
	{
		auto threshold = getSettingAndValidate<ConfigurationType::Float>(configuration,
			Settings::Threshold,
			ConfigurationType::Type::Float).getValue();
		return new FloydSteinberg(*this, processorToRun, threshold);
	}

	FloydSteinbergSpecification::FloydSteinbergSpecification()
		: TaskSpecification("Floyd-Steinberg Dithering",
		ResultSpecification("Conversion Rate",
			"Pixels",
			ResultSpecification::Throughput,
			ResultSpecification::Accumulative))
	{

		withDescription("Converts an Gray Scale image to Black-White dithered with Floyd Steinbergs algorithm");

		withInputData(DataSpecification("Input Image Data",
			"Pixels",
			4,
			{ "width", "height" },
			"Input image data in Grayscale and RGBA float pixels"));
		withOutputData(DataSpecification("Output Image Data",
			"Pixels",
			4,
			{ "width", "height" },
			"Output image data in Black-White dithered and RGBA float pixels"));

		withConfiguration(new ConfigurationSpecification<ConfigurationType::Float>(0.5,
			"Threshold",
			"The threshold to use for determining if a pixel should be black or white",
			true));

	}
}