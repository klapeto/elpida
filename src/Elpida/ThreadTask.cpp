//
// Created by klapeto on 28/2/2023.
//

#include "ThreadTask.hpp"

namespace Elpida
{
	void ThreadTask::Prepare(TaskData&& inputData)
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

	TaskData ThreadTask::Finalize()
	{
		return _taskToRun->Finalize();
	}

	bool ThreadTask::CanBeMultiThreaded() const
	{
		return false;
	}

	ThreadTask::ThreadTask(std::unique_ptr<Task> taskToRun, int processorId)
		: _taskToRun(std::move(taskToRun)), _taskRunDuration(0), _processorId(processorId), _doStart(false)
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

	std::unique_ptr<Task> ThreadTask::DoDuplicate() const
	{
		return std::unique_ptr<Task>(new ThreadTask(_taskToRun->Duplicate(), _processorId));
	}

	TaskInfo ThreadTask::GetInfo() const
	{
		return _taskToRun->GetInfo();
	}

	void ThreadTask::ThreadProcedure()
	{
		PinCurrentThreadToProcessor(_processorId);

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