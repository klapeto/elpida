//
// Created by klapeto on 7/4/2024.
//

#include "ParseXmlTask.hpp"
#include "Elpida/Core/SimpleTaskData.hpp"
#include "Elpida/Xml/XmlParser.hpp"

namespace Elpida
{

	TaskInfo ParseXmlTask::DoGetInfo() const
	{
		return { "XML Parsing",
				 "Parses an XML document and measures the parsing throughput",
				 "chars",
				 "How many characters are processes in the time",
				 ResultType::Throughput};
	}

	UniquePtr<Task> ParseXmlTask::DoDuplicate() const
	{
		return std::make_unique<ParseXmlTask>();
	}

	void ParseXmlTask::Prepare(SharedPtr<AbstractTaskData> inputData)
	{
		_inputData = std::move(inputData);
	}

	SharedPtr<AbstractTaskData> ParseXmlTask::Finalize()
	{
		return std::make_shared<SimpleTaskData<XmlElement>>(std::move(_parsedElement), _inputData->GetAllocator());
	}

	Size ParseXmlTask::GetProcessedDataSize() const
	{
		return _inputData->GetSize();
	}

	void ParseXmlTask::DoRun(Iterations iterations)
	{
		const char* ptr = reinterpret_cast<char*>(_inputData.get()->GetData());
		auto size = _inputData->GetSize();
		while (iterations-- > 0)
		{
			_parsedElement = XmlParser::Parse(ptr, size);
		}
	}

	Size ParseXmlTask::GetOperationsPerformedPerRun()
	{
		return 1;
	}

	Duration ParseXmlTask::GetExecutionMinimumDuration()
	{
		return Elpida::Seconds(5);
	}
} // Elpida