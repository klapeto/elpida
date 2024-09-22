#include "SqrtTask.hpp"

#include <cmath>
#include "Elpida/Core/Repeat.hpp"

namespace Elpida
{
	void SqrtTask::Prepare(SharedPtr<AbstractTaskData> inputData)
	{
		_inputData = std::move(inputData);
	}

	SharedPtr<AbstractTaskData> SqrtTask::Finalize()
	{
		return std::move(_inputData);
	}

	Size SqrtTask::GetProcessedDataSize() const
	{
		return 1;
	}

	void SqrtTask::DoRunImpl()
	{
		double x = 156113132146186435.1513123;
		Exec([&]()
		{
			REPEAT_10000(x = std::sqrt(x));
		});
	}

	Size SqrtTask::GetOperationsPerformedPerRun()
	{
		return 10000;
	}

	Duration SqrtTask::GetExecutionMinimumDuration()
	{
		return Elpida::Seconds(5);
	}

	UniquePtr<Task> SqrtTask::DoDuplicate() const
	{
		return std::make_unique<SqrtTask>();
	}

	TaskInfo SqrtTask::DoGetInfo() const
	{
		return { "sqrt()",
				 "Measures the sqrt() performance",
				 "numbers",
				 "The number process rate",
				 ResultType::Throughput
		};
	}

}