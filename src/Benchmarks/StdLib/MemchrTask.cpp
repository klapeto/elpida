#include "MemchrTask.hpp"

#include <cstring>

namespace Elpida
{

	const char Character = 'o';

	void MemchrTask::Prepare(SharedPtr<AbstractTaskData> inputData)
	{
		_inputData = std::move(inputData);
		std::memset(_inputData->GetData(), 0, _inputData->GetSize());
		_inputData->GetData()[_inputData->GetSize() - 1] = Character;
	}

	SharedPtr<AbstractTaskData> MemchrTask::Finalize()
	{
		return std::move(_inputData);
	}

	Size MemchrTask::GetProcessedDataSize() const
	{
		return _inputData->GetSize();
	}

	void MemchrTask::DoRun(Iterations iterations)
	{
		auto data = _inputData->GetData();
		auto size = _inputData->GetSize();
		while (iterations-- > 0)
		{
			volatile auto x = std::memchr(data, Character, size);
		}
	}

	Size MemchrTask::GetOperationsPerformedPerRun()
	{
		return 1;
	}

	Duration MemchrTask::GetExecutionMinimumDuration()
	{
		return Elpida::Seconds(5);
	}

	UniquePtr<Task> MemchrTask::DoDuplicate() const
	{
		return std::make_unique<MemchrTask>();
	}

	TaskInfo MemchrTask::DoGetInfo() const
	{
		return { "Memchr",
				 "Memchr Description",
				 "B",
				 "The process rate",
				 ResultType::Throughput
		};
	}

}