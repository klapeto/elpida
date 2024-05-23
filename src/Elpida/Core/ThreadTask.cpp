//
// Created by klapeto on 28/2/2023.
//

#include "Elpida/Core/ThreadTask.hpp"

namespace Elpida
{
	void ThreadTask::Prepare(SharedPtr<AbstractTaskData> inputData)
	{
		_taskToRun->Prepare(std::move(inputData));

		std::unique_lock<std::mutex> lock(_mutex);
		_doStart = false;
		_thread = std::thread(&ThreadTask::ThreadProcedure, this);
	}

	Duration ThreadTask::Run()
	{
		WakeThread();
		_thread.join();

		if (_exception)
		{
			rethrow_exception(_exception);
		}

		return _taskRunDuration;
	}

	SharedPtr<AbstractTaskData> ThreadTask::Finalize()
	{
		return _taskToRun->Finalize();
	}

	ThreadTask::ThreadTask(UniquePtr<Task> taskToRun, Optional<Ref<const ProcessingUnitNode>> targetProcessor)
			: _taskToRun(std::move(taskToRun)), _targetProcessor(targetProcessor), _taskRunDuration(0), _doStart(false)
	{

	}

	ThreadTask::~ThreadTask()
	{
		WakeThread();
		if (_thread.joinable())
		{
			_thread.join();
		}
	}

	void ThreadTask::DoRun()
	{

	}

	UniquePtr<Task> ThreadTask::DoDuplicate() const
	{
		return UniquePtr<Task>(new ThreadTask(_taskToRun->Duplicate(), _targetProcessor));
	}

	TaskInfo ThreadTask::DoGetInfo() const
	{
		return _taskToRun->GetInfo();
	}

	void ThreadTask::ThreadProcedure()
	{
		try
		{
			if (_targetProcessor.has_value())
			{
				_targetProcessor->get().PinThreadToThisProcessor();
			}
			std::unique_lock<std::mutex> lock(_mutex);
			_conditionVariable.wait(lock, [this]()
			{
				return _doStart;
			});
			_taskRunDuration = _taskToRun->Run();
		}
		catch (...)
		{
			_exception = std::current_exception();
		}
	}

	void ThreadTask::WakeThread()
	{
		std::unique_lock<std::mutex> lock(_mutex);
		_doStart = true;
		_conditionVariable.notify_all();
	}

	Size ThreadTask::GetProcessedDataSize() const
	{
		return _taskToRun->GetProcessedDataSize();
	}

} // Elpida