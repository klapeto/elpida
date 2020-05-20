//
// Created by klapeto on 20/4/20.
//

#include <cmath>
#include "Elpida/Engine/Task/MultiThreadTask.hpp"
#include "Elpida/Topology/ProcessorNode.hpp"
#include "Elpida/Config.hpp"
#include "Elpida/Engine/Task/TaskSpecification.hpp"
#include "Elpida/Engine/Data/PassiveTaskData.hpp"
#include "Elpida/Engine/Task/TaskBuilder.hpp"

namespace Elpida
{

	MultiThreadTask::MultiThreadTask(const TaskBuilder& taskBuilder,
		const TaskConfiguration& configuration,
		const TaskAffinity& affinity)
		: Task(taskBuilder.getTaskSpecification(), affinity), _configuration(configuration), _threadsShouldWake(false)
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

		_createdThreads.reserve(processorCount);
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

	double MultiThreadTask::calculateTaskResultValue(const Duration& taskElapsedTime) const
	{
		double accumulator = 0;
		for (auto& thread: _createdThreads)
		{
			accumulator += thread.getTaskToRun().calculateTaskResultValue(taskElapsedTime);
		}
		return _specification.getResultSpecification().getAggregationType() == ResultSpecification::Accumulative
			   ? accumulator : accumulator / _createdThreads.size();
	}

}