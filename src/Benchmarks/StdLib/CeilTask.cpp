#include "CeilTask.hpp"

#include <cmath>
#include "Elpida/Core/Repeat.hpp"

namespace Elpida
{

	void CeilTask::Prepare(SharedPtr<AbstractTaskData> inputData)
	{
		_inputData = std::move(inputData);
	}

	SharedPtr<AbstractTaskData> CeilTask::Finalize()
	{
		return std::move(_inputData);
	}

	Size CeilTask::GetProcessedDataSize() const
	{
		return 1;
	}

	void CeilTask::DoRun(Iterations iterations)
	{
		double y = 1561131321.1513123;
		while (iterations-- > 0)
		{
			REPEAT_10000(y = std::ceil(y));
		}
	}

	Size CeilTask::GetOperationsPerformedPerRun()
	{
		return 10000;
	}

	Duration CeilTask::GetExecutionMinimumDuration()
	{
		return Elpida::Seconds(5);
	}

	UniquePtr<Task> CeilTask::DoDuplicate() const
	{
		return std::make_unique<CeilTask>();
	}

	TaskInfo CeilTask::DoGetInfo() const
	{
		return { "ceil()",
				 "Measures the ceil() performance",
				 "numbers",
				 "The number process rate",
				 ResultType::Throughput
		};
	}

}