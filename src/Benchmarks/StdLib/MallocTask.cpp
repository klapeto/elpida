#include "MallocTask.hpp"

#include <cstring>
#include <random>

namespace Elpida
{
	void MallocTask::Prepare(SharedPtr<AbstractTaskData> inputData)
	{
		_inputData = std::move(inputData);
	}

	SharedPtr<AbstractTaskData> MallocTask::Finalize()
	{
		for (auto ptr : _pointers)
		{
			free(ptr);
		}

		_pointers.clear();

		return std::move(_inputData);
	}

	Size MallocTask::GetProcessedDataSize() const
	{
		return _totalSize;
	}

	void MallocTask::DoRun(Iterations iterations)
	{
		while (iterations-- > 0)
		{
			auto size = _sizes[iterations];
			auto ptr = malloc(size);
			std::memset(ptr, 7, size);
			_pointers.push_back(ptr);
		}
	}

	Size MallocTask::GetOperationsPerformedPerRun()
	{
		return 1;
	}

	Duration MallocTask::GetExecutionMinimumDuration()
	{
		return Elpida::MilliSeconds(100);
	}

	UniquePtr<Task> MallocTask::DoDuplicate() const
	{
		return std::make_unique<MallocTask>();
	}

	TaskInfo MallocTask::DoGetInfo() const
	{
		return { "Malloc rate",
				 "Measures the allocation rate",
				 "B",
				 "The allocation rate",
				 ResultType::Throughput
		};
	}

	void MallocTask::OnBeforeRun(Iterations iterations)
	{
		_sizes.reserve(iterations);
		_sizes.clear();
		auto generator = std::mt19937((std::size_t)this);
		auto distribution = std::uniform_int_distribution<std::size_t>(32, 64);

		_totalSize = 0;
		for (std::size_t i = 0; i < iterations; ++i)
		{
			auto size = distribution(generator);
			_totalSize += size;
			_sizes.push_back(size);
		}

		_pointers.reserve(iterations + _pointers.size());
	}

	MallocTask::MallocTask()
	: _totalSize(0)
	{

	}
}