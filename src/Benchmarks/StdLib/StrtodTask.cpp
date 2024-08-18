#include <random>
#include "StrtodTask.hpp"

namespace Elpida
{

	static const char* Numbers[] = {
			"53215.1513123",
			"46584.2315456",
			"11345.1237488",
			"94543.7348612",
			"87331.2332564",
	};

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
			auto value = std::strtod(Numbers[iterations % std::size(Numbers)], nullptr);
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
		return { "Strtod",
				 "Measures the strtod() performance",
				 "chars",
				 "The character process rate",
				 ResultType::Throughput
		};
	}
}