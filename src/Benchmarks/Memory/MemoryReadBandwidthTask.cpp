//
// Created by klapeto on 28/2/2023.
//

#include "MemoryReadBandwidthTask.hpp"

namespace Elpida
{
	void MemoryReadBandwidthTask::Prepare(SharedPtr<AbstractTaskData> inputData)
	{
		_data = std::move(inputData);
		_ptr = _data->GetData();
	}

	SharedPtr<AbstractTaskData> MemoryReadBandwidthTask::Finalize()
	{
		return std::move(_data);
	}

	TaskInfo MemoryReadBandwidthTask::DoGetInfo() const
	{
		return {
				"Memory read bandwidth",
				"Continuously reads a stream a memory to determine th maximum memory read bandwidth.",
				"B",
				"The memory bandwidth throughput",
				ResultType::Throughput
		};
	}

	MemoryReadBandwidthTask::MemoryReadBandwidthTask()
		: _ptr(nullptr)
	{

	}

	void MemoryReadBandwidthTask::DoRun(Iterations iterations)
	{
		volatile auto* ptr = (int*)_ptr;
		const auto size = _data->GetSize();

		while (iterations-- > 0)
		{
			for (auto j = 0ul; j < size / sizeof(int); j += 32)
			{
				ptr[j];
				ptr[j + 1];
				ptr[j + 2];
				ptr[j + 3];
				ptr[j + 4];
				ptr[j + 5];
				ptr[j + 6];
				ptr[j + 7];
				ptr[j + 8];
				ptr[j + 9];
				ptr[j + 10];
				ptr[j + 11];
				ptr[j + 12];
				ptr[j + 13];
				ptr[j + 14];
				ptr[j + 15];
				ptr[j + 16];
				ptr[j + 17];
				ptr[j + 18];
				ptr[j + 19];
				ptr[j + 20];
				ptr[j + 21];
				ptr[j + 22];
				ptr[j + 23];
				ptr[j + 24];
				ptr[j + 25];
				ptr[j + 26];
				ptr[j + 27];
				ptr[j + 28];
				ptr[j + 29];
				ptr[j + 30];
				ptr[j + 31];
			}
		}
	}

	Size MemoryReadBandwidthTask::GetOperationsPerformedPerRun()
	{
		return 1;
	}

	Duration MemoryReadBandwidthTask::GetExecutionMinimumDuration()
	{
		return Seconds(5);
	}

	UniquePtr<Task> MemoryReadBandwidthTask::DoDuplicate() const
	{
		return std::unique_ptr<Task>(new MemoryReadBandwidthTask());
	}

	Size MemoryReadBandwidthTask::GetProcessedDataSize() const
	{
		return _data->GetSize();
	}
} // Elpida