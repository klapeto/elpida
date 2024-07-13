//
// Created by klapeto on 21/5/2024.
//

#include "XmlGenerationTask.hpp"

#include <cmath>
#include <cstring>

namespace Elpida
{
	static const char Header[] = R"(<root>)";
	static const char Element[] = R"(<element attr="test"><child id="a"/><child id="b"/></element>)";
	static const char Footer[] = R"(</root>)";

	XmlGenerationTask::XmlGenerationTask(size_t xmlSize)
			: _elementCount(1), _xmlSize(xmlSize)
	{
	}

	void XmlGenerationTask::Prepare(SharedPtr<AbstractTaskData> inputData)
	{
		_outputData = std::make_unique<RawTaskData>(inputData->GetAllocator());

		_elementCount = std::max(1.0, std::ceil(_xmlSize / static_cast<double>(sizeof(Element))));
		_xmlSize = sizeof(Header) + _elementCount * sizeof(Element) + sizeof(Footer);
		_outputData->Allocate(_xmlSize);
	}

	SharedPtr<AbstractTaskData> XmlGenerationTask::Finalize()
	{
		return std::move(_outputData);
	}

	Size XmlGenerationTask::GetProcessedDataSize() const
	{
		return _xmlSize;
	}

	TaskInfo XmlGenerationTask::DoGetInfo() const
	{
		return TaskInfo("Xml generation",
				"Generates a xml file in memory with the provided size",
				"B",
				"The rate of generation",
				ResultType::Throughput);
	}

	void XmlGenerationTask::DoRun()
	{
		auto ptr = _outputData->GetData();
		std::memcpy(ptr, Header, sizeof(Header) - 1);
		ptr += sizeof(Header) - 1;

		for (std::size_t i = 0; i < _elementCount; ++i)
		{
			std::memcpy(ptr, Element, sizeof(Element) - 1);
			ptr += sizeof(Element) - 1;
		}

		std::memcpy(ptr, Footer, sizeof(Footer) - 1);
	}

	UniquePtr<Task> XmlGenerationTask::DoDuplicate() const
	{
		return std::make_unique<XmlGenerationTask>(_xmlSize);
	}
} // Elpida