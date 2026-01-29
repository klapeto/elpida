/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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

	ThreadTask::ThreadTask(UniquePtr<Task> taskToRun, Optional<Ref<const ProcessingUnitNode>> targetProcessor,const TopologyInfo& topologyInfo)
			: _taskToRun(std::move(taskToRun)), _topologyInfo(topologyInfo), _targetProcessor(targetProcessor), _taskRunDuration(0), _doStart(false)
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
		return UniquePtr<Task>(new ThreadTask(_taskToRun->Duplicate(), _targetProcessor, _topologyInfo));
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
				_topologyInfo.PinThreadToProcessor(_targetProcessor->get().GetOsIndex().value());
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
		if (_doStart) return;
		std::unique_lock<std::mutex> lock(_mutex);
		_doStart = true;
		_conditionVariable.notify_all();
	}

	Size ThreadTask::GetProcessedDataSize() const
	{
		return _taskToRun->GetProcessedDataSize();
	}

} // Elpida