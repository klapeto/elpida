#include "MemsetTask.hpp"

extern "C" void *memset(void *dest, int c, size_t n);

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
		return _inputData->GetSize();
	}

	void MemsetTask::DoRun(Iterations iterations)
	{
		volatile auto data = _inputData->GetData();
		auto size = _inputData->GetSize();
		while (iterations-- > 0)
		{
			memset(data, 0, size);
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