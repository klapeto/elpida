#include "FloorTask.hpp"

#include "Elpida/Core/Repeat.hpp"

namespace Elpida
{

	void FloorTask::Prepare(SharedPtr<AbstractTaskData> inputData)
	{
		_inputData = std::move(inputData);
	}

	SharedPtr<AbstractTaskData> FloorTask::Finalize()
	{
		return std::move(_inputData);
	}

	Size FloorTask::GetProcessedDataSize() const
	{
		return 1;
	}

	void FloorTask::DoRunImpl()
	{
		double y = 1561131321.1513123;
		Exec([&]()
		{
			REPEAT_10000(y = std::floor(y));
		});
	}

	Size FloorTask::GetOperationsPerformedPerRun()
	{
		return 10000;
	}

	Duration FloorTask::GetExecutionMinimumDuration()
	{
		return Elpida::Seconds(5);
	}

	UniquePtr<Task> FloorTask::DoDuplicate() const
	{
		return std::make_unique<FloorTask>();
	}

	TaskInfo FloorTask::DoGetInfo() const
	{
		return { "floor()",
				 "Measures the floor() performance",
				 "numbers",
				 "The number process rate",
				 ResultType::Throughput
		};
	}

}