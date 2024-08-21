#include "SqrtTask.hpp"

#include "Elpida/Core/Repeat.hpp"

extern "C" double sqrt(double x);

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

	void SqrtTask::DoRun(Iterations iterations)
	{
		double x = 156113132146186435.1513123;
		while (iterations-- > 0)
		{
			REPEAT_10000(x = sqrt(x));
		}
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