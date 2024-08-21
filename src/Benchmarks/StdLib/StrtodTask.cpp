#include "StrtodTask.hpp"

extern "C" double strtod(const char* s, char** p);

namespace Elpida
{
	void StrtodTask::Prepare(SharedPtr<AbstractTaskData> inputData)
	{
		_inputData = std::move(inputData);
	}

	SharedPtr<AbstractTaskData> StrtodTask::Finalize()
	{
		return std::move(_inputData);
	}

	Size StrtodTask::GetProcessedDataSize() const
	{
		return sizeof("53215.1513123");
	}

	void StrtodTask::DoRun(Iterations iterations)
	{
		while (iterations-- > 0)
		{
			auto value = strtod("53215.1513123", nullptr);
		}
	}

	Size StrtodTask::GetOperationsPerformedPerRun()
	{
		return 1;
	}

	Duration StrtodTask::GetExecutionMinimumDuration()
	{
		return Elpida::Seconds(5);
	}

	UniquePtr<Task> StrtodTask::DoDuplicate() const
	{
		return std::make_unique<StrtodTask>();
	}

	TaskInfo StrtodTask::DoGetInfo() const
	{
		return { "strtod()",
				 "Measures the strtod() performance",
				 "chars",
				 "The character process rate",
				 ResultType::Throughput
		};
	}
}