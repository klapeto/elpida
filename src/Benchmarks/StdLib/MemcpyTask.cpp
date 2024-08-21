#include "MemcpyTask.hpp"

#undef memcpy
extern "C" void* memcpy(void* dest, const void* src, size_t n);

namespace Elpida
{

	void MemcpyTask::Prepare(SharedPtr<AbstractTaskData> inputData)
	{
		_inputData = std::move(inputData);
		_outputData = std::make_shared<RawTaskData>(_inputData->GetAllocator());
		_outputData->Allocate(_inputData->GetSize());
	}

	SharedPtr<AbstractTaskData> MemcpyTask::Finalize()
	{
		return std::move(_outputData);
	}

	Size MemcpyTask::GetProcessedDataSize() const
	{
		return _inputData->GetSize();
	}

	void MemcpyTask::DoRun(Iterations iterations)
	{
		volatile auto data = _inputData->GetData();
		auto size = _inputData->GetSize();
		auto out = _outputData->GetData();
		while (iterations-- > 0)
		{
			memcpy(out, data, size);
		}
	}

	Size MemcpyTask::GetOperationsPerformedPerRun()
	{
		return 1;
	}

	Duration MemcpyTask::GetExecutionMinimumDuration()
	{
		return Elpida::Seconds(5);
	}

	UniquePtr<Task> MemcpyTask::DoDuplicate() const
	{
		return std::make_unique<MemcpyTask>();
	}

	TaskInfo MemcpyTask::DoGetInfo() const
	{
		return { "memcpy()",
				 "Measures the memcpy() performance",
				 "B",
				 "The process rate",
				 ResultType::Throughput
		};
	}

}