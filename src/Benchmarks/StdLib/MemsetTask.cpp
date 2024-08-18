#include "MemsetTask.hpp"

namespace Elpida
{

	void MemsetTask::Prepare(SharedPtr<AbstractTaskData> inputData)
	{
		_inputData = std::move(inputData);
	}

	SharedPtr<AbstractTaskData> MemsetTask::Finalize()
	{
		return std::move(_inputData);
	}

	Size MemsetTask::GetProcessedDataSize() const
	{
		return 1;
	}

	void MemsetTask::DoRun(Iterations iterations)
	{
		while (iterations-- > 0)
		{
			// Code
		}
	}

	Size MemsetTask::GetOperationsPerformedPerRun()
	{
		return 1;
	}

	Duration MemsetTask::GetExecutionMinimumDuration()
	{
		return Elpida::Seconds(5);
	}

	UniquePtr<Task> MemsetTask::DoDuplicate() const
	{
		return std::make_unique<MemsetTask>();
	}

	TaskInfo MemsetTask::DoGetInfo() const
	{
		return { "Memset",
				 "Memset Description",
				 "B",
				 "The process rate",
				 ResultType::Throughput
		};
	}

}