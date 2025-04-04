//
// Created by klapeto on 7/4/2024.
//

#include "ParseJsonTask.hpp"
#include "Elpida/Core/SimpleTaskData.hpp"

namespace Elpida
{
	void ParseJsonTask::Prepare(SharedPtr<AbstractTaskData> inputData)
	{
		_inputData = std::move(inputData);
	}

	SharedPtr<AbstractTaskData> ParseJsonTask::Finalize()
	{
		return std::make_shared<SimpleTaskData<nlohmann::json>>(std::move(_parsedElement), _inputData->GetAllocator());
	}

	TaskInfo ParseJsonTask::DoGetInfo() const
	{
		return { "JSON Parsing",
				 "Parses an JSON document and measures the parsing throughput",
				 "chars",
				 "How many characters are processes in the time",
				 ResultType::Throughput
		};
	}

	Size ParseJsonTask::GetProcessedDataSize() const
	{
		return _inputData->GetSize();
	}

	void ParseJsonTask::DoRunImpl()
	{
		const char* ptr = reinterpret_cast<char*>(_inputData.get()->GetData());
		std::string_view str{ ptr, _inputData->GetSize() };

		Exec([&]()
		{
			_parsedElement = nlohmann::json::parse(str);
		});
	}

	Size ParseJsonTask::GetOperationsPerformedPerRun()
	{
		return 1;
	}

	UniquePtr<Task> ParseJsonTask::DoDuplicate() const
	{
		return std::make_unique<ParseJsonTask>();
	}
} // Elpida