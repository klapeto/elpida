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
		return _inputData->GetSize();
	}

	void ${NAME}Task::DoRunImpl()
	{
		auto data = _inputData->GetData();
		auto size = _inputData->GetSize();

		Exec([&]()
		{
			// Code
		});
	}

	Size ${NAME}Task::GetOperationsPerformedPerRun()
	{
		return 1;
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