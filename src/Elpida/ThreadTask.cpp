//
// Created by klapeto on 28/2/2023.
//

#include "Elpida/ThreadTask.hpp"

namespace Elpida
{
	void ThreadTask::Prepare(UniquePtr<AbstractTaskData> inputData)
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

		return _taskRunDuration;
	}

	UniquePtr<AbstractTaskData> ThreadTask::Finalize()
	{
		return _taskToRun->Finalize();
	}

	bool ThreadTask::CanBeMultiThreaded() const
	{
		return false;
	}

	ThreadTask::ThreadTask(UniquePtr<Task> taskToRun, Ref<const TopologyNode> targetProcessor)
		: _taskToRun(std::move(taskToRun)), _taskRunDuration(0), _targetProcessor(targetProcessor), _doStart(false)
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

	TaskInfo ThreadTask::GetInfo() const
	{
		return _taskToRun->GetInfo();
	}

	void ThreadTask::ThreadProcedure()
	{
		PinCurrentThreadToProcessor(_targetProcessor);

		std::unique_lock<std::mutex> lock(_mutex);
		_conditionVariable.wait(lock, [this]()
		{
			return _doStart;
		});
		_taskRunDuration = _taskToRun->Run();
	}

	void ThreadTask::WakeThread()
	{
		std::unique_lock<std::mutex> lock(_mutex);
		_doStart = true;
		_conditionVariable.notify_all();
	}
} // Elpida