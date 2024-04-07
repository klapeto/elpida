//
// Created by klapeto on 7/4/2024.
//

#include "ParseJsonTask.hpp"
#include "Elpida/Core/SimpleTaskData.hpp"

namespace Elpida
{
	void ParseJsonTask::Prepare(UniquePtr<AbstractTaskData> inputData)
	{
		_inputData = std::move(inputData);
	}

	bool ParseJsonTask::CanBeMultiThreaded() const
	{
		return false;
	}

	UniquePtr<AbstractTaskData> ParseJsonTask::Finalize()
	{
		return std::make_unique<SimpleTaskData<nlohmann::json>>(std::move(_parsedElement), _inputData->GetAllocator());
	}

	TaskInfo ParseJsonTask::DoGetInfo() const
	{
		return { "JSON Parsing",
				 "Parses an JSON document and measures the parsing throughput",
				 "chars",
				 "How many characters are processes in the time",
				 ScoreType::Throughput
		};
	}

	Size ParseJsonTask::GetProcessedDataSize() const
	{
		return _inputData->GetSize();
	}

	void ParseJsonTask::DoRun(Iterations iterations)
	{
		const char* ptr = reinterpret_cast<char*>(_inputData.get()->GetData());
		std::string_view str{ptr, _inputData->GetSize()};
		while (iterations-- > 0)
		{
			_parsedElement = nlohmann::json::parse(str);
		}
	}

	Size ParseJsonTask::GetOperationsPerformedPerRun()
	{
		return 1;
	}

	Duration ParseJsonTask::GetExecutionMinimumDuration()
	{
		return Elpida::Seconds(5);
	}

	UniquePtr<Task> ParseJsonTask::DoDuplicate() const
	{
		return std::make_unique<ParseJsonTask>();
	}
} // Elpida