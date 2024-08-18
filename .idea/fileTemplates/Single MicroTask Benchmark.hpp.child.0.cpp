#[[#include]]# "${NAME}Task.hpp"

namespace Elpida {

	void ${NAME}Task::Prepare(SharedPtr<AbstractTaskData> inputData)
	{
		_inputData = std::move(inputData);
	}

	SharedPtr<AbstractTaskData> ${NAME}Task::Finalize()
	{
		return std::move(_inputData);
	}

	Size ${NAME}Task::GetProcessedDataSize() const
	{
		return 1;
	}

	void ${NAME}Task::DoRun(Iterations iterations)
	{
		while (iterations-- > 0)
		{
			// Code
		}
	}

	Size ${NAME}Task::GetOperationsPerformedPerRun()
	{
		return 1;
	}

	Duration ${NAME}Task::GetExecutionMinimumDuration()
	{
		return Elpida::Seconds(5);
	}

	UniquePtr<Task> ${NAME}Task::DoDuplicate() const
	{
		return std::make_unique<${NAME}Task>();
	}

	TaskInfo ${NAME}Task::DoGetInfo() const
	{
		return { "${NAME}",
				 "${NAME} Description",
				 "B",
				 "The process rate",
				 ResultType::Throughput
		};
	}

}