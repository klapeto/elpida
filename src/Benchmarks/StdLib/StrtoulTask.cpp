#include "StrtoulTask.hpp"

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
		return sizeof("1524515736");
	}

	void StrtoulTask::DoRunImpl()
	{
		Exec([&]()
		{
			auto value = std::strtoul("1524515736", nullptr, 10);
		});
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