//
// Created by klapeto on 20/4/20.
//

#include <cmath>
#include "Elpida/Engine/Task/MultiThreadTask.hpp"
#include "Elpida/Topology/ProcessorNode.hpp"
#include "Elpida/Config.hpp"
#include "Elpida/Engine/Task/Data/TaskData.hpp"
#include "Elpida/Engine/Task/TaskSpecification.hpp"
#include "Elpida/Engine/Task/Data/PassiveTaskData.hpp"

namespace Elpida
{

	MultiThreadTask::MultiThreadTask(const TaskSpecification& taskSpecification,
		const TaskConfiguration& configuration,
		const TaskAffinity& affinity)
		: Task(taskSpecification, affinity, true), _configuration(configuration), _threadsShouldWake(false)
	{
	}

	void MultiThreadTask::execute()
	{
		std::unique_lock<std::mutex> lock(_mutex);
		_threadsShouldWake = true;
		lock.unlock();
		_wakeNotifier.notify_all();
		for (auto& thread : _createdThreads)
		{
			thread.join();
		}
	}

	void MultiThreadTask::applyAffinity()
	{

	}

	void MultiThreadTask::prepareImpl()
	{
		const auto& processors = _affinity.getProcessorNodes();
		const size_t processorCount = processors.size();

		auto& input = getInput();
		size_t i = 0;
		for (auto processor : processors)
		{
			auto task = _specification
				.createNewTask(_configuration, TaskAffinity(std::vector<const ProcessorNode*>{ processor }));

			if (_specification.acceptsInput())
			{
				auto& data = input.getTaskData()[i];
				task->setInput(TaskInput({ new PassiveTaskData(data->getData(), data->getSize()) }));
			}

			task->prepare();
			_createdThreads.push_back(std::move(TaskThread(*task,
				_wakeNotifier,
				_mutex,
				_threadsShouldWake,
				processor->getOsIndex())));
			_createdThreads.back().start();
		}
	}

	TaskOutput MultiThreadTask::finalizeAndGetOutputData()
	{
		std::vector<TaskData*> accumulatedOutputs;
		for (auto& thread: _createdThreads)
		{
			thread.getTaskToRun().finalize();
			for (auto& out : thread.getTaskToRun().getOutput().getTaskData())
			{
				accumulatedOutputs.push_back(new PassiveTaskData(*out));
			}
		}

		return TaskOutput(std::move(accumulatedOutputs));
	}
}