//
// Created by klapeto on 31/5/20.
//

#include "Benchmarks/Image/ImageTaskBase.hpp"

#include <Elpida/Config.hpp>
#include <Elpida/ElpidaException.hpp>

namespace Elpida
{

	ImageTaskBase::InputImageProperties ImageTaskBase::getImageProperties(const TaskDataDto& taskData)
	{
		auto properties = taskData.getDefinedProperties();
		try
		{
			auto width = properties.at("width");
			auto height = properties.at("height");
			return { (size_t)width, (size_t)height };
		}
		catch (const std::out_of_range& ex)
		{
			throw ElpidaException(FUNCTION_NAME, "Required properties were not defined!");
		}
	}

	ImageTaskBase::ImageTaskBase(const TaskSpecification& specification, const ProcessorNode& processorToRun)
		: Task(specification, processorToRun)
	{

	}
}