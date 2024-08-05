//
// Created by klapeto on 2/6/2024.
//

#include "CommonTasks/AllocateMemoryTask.hpp"

namespace Elpida
{
	void AllocateMemoryTask::Prepare(SharedPtr<AbstractTaskData> inputData)
	{
		_outputData = std::make_unique<RawTaskData>(inputData->GetAllocator());
	}

	SharedPtr<AbstractTaskData> AllocateMemoryTask::Finalize()
	{
		return std::move(_outputData);
	}

	ConcurrencyMode AllocateMemoryTask::GetAllowedConcurrency() const
	{
		return ConcurrencyMode::None;
	}

	Size AllocateMemoryTask::GetProcessedDataSize() const
	{
		return _size;
	}

	AllocateMemoryTask::AllocateMemoryTask(std::size_t size)
			:_size(size)
	{

	}

	void AllocateMemoryTask::DoRun()
	{
		_outputData->Allocate(_size);
	}

	std::unique_ptr<Task> AllocateMemoryTask::DoDuplicate() const
	{
		return std::make_unique<AllocateMemoryTask>(_size);
	}

	TaskInfo AllocateMemoryTask::DoGetInfo() const
	{
		return {
				"Allocate memory",
				"Allocates a chunk of memory to be used by other tasks.",
				"B",
				"The amount of bytes allocated per second",
				ResultType::Throughput
		};
	}
} // Elpida