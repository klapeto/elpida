//
// Created by klapeto on 7/4/2024.
//

#include "SvgParseTask.hpp"
#include "Elpida/Core/SimpleTaskData.hpp"

namespace Elpida
{
	void SvgParseTask::Prepare(UniquePtr<AbstractTaskData> inputData)
	{
		_inputData = std::move(inputData);
		auto ptr = dynamic_cast<SimpleTaskData<XmlElement>*>(_inputData.get());

		_inputXmlElement = std::move(ptr->GetUnderlyingData());
	}

	bool SvgParseTask::CanBeMultiThreaded() const
	{
		return false;
	}

	UniquePtr<AbstractTaskData> SvgParseTask::Finalize()
	{
		return std::make_unique<SimpleTaskData<SvgDocument>>(std::move(_parsedElement), _inputData->GetAllocator());
	}

	TaskInfo SvgParseTask::DoGetInfo() const
	{
		return { "SVG Parsing",
				 "Parses a SVG document and measures the parsing throughput",
				 "elements",
				 "How many elements are processes in the time",
				 ScoreType::Throughput
		};
	}

	static Size GetElementCount(const XmlElement& element)
	{
		Size elements = element.GetChildren().size() + 1;

		for (auto& child: element.GetChildren())
		{
			elements += GetElementCount(child);
		}
		return elements;
	}

	Size SvgParseTask::GetProcessedDataSize() const
	{
		return GetElementCount(_inputXmlElement);
	}

	void SvgParseTask::DoRun(Iterations iterations)
	{
		auto& element = _inputXmlElement;
		while (iterations-- > 0)
		{
			_parsedElement = SvgDocument(element);
		}
	}

	Size SvgParseTask::GetOperationsPerformedPerRun()
	{
		return 1;
	}

	Duration SvgParseTask::GetExecutionMinimumDuration()
	{
		return Elpida::Seconds(5);
	}

	UniquePtr<Task> SvgParseTask::DoDuplicate() const
	{
		return std::make_unique<SvgParseTask>();
	}
} // Elpida