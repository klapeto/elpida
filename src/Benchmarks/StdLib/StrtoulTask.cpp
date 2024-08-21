#include "StrtoulTask.hpp"

extern "C" unsigned long strtoul(const char *s, char **p, int base);

namespace Elpida
{

	void StrtoulTask::Prepare(SharedPtr<AbstractTaskData> inputData)
	{
		_inputData = std::move(inputData);
	}

	SharedPtr<AbstractTaskData> StrtoulTask::Finalize()
	{
		return std::move(_inputData);
	}

	Size StrtoulTask::GetProcessedDataSize() const
	{
		return sizeof("5321515131");
	}

	void StrtoulTask::DoRun(Iterations iterations)
	{
		while (iterations-- > 0)
		{
			auto value = strtoul("1524515736", nullptr, 10);
		}
	}

	Size StrtoulTask::GetOperationsPerformedPerRun()
	{
		return 1;
	}

	Duration StrtoulTask::GetExecutionMinimumDuration()
	{
		return Elpida::Seconds(5);
	}

	UniquePtr<Task> StrtoulTask::DoDuplicate() const
	{
		return std::make_unique<StrtoulTask>();
	}

	TaskInfo StrtoulTask::DoGetInfo() const
	{
		return { "Strtoul",
				 "Measures the strtoul() performance",
				 "chars",
				 "The character process rate",
				 ResultType::Throughput
		};
	}

}