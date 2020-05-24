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
		: Task(taskBuilder.getTaskSpecification(), affinity), _configuration(configuration), _taskBuilder(taskBuilder),
		  _threadsShouldWake(false)
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
		auto& adapter = _taskBuilder.getDataAdapter();
		const auto& processors = _affinity.getProcessorNodes();
		const size_t processorCount = processors.size();

		auto& input = getInput();
		auto newData =
			adapter.breakIntoChunks(*input.getTaskData(), _affinity, _specification.getInputDataSpecification());

		_createdThreads.reserve(processorCount);

		size_t i = 0;
		for (auto processor : processors)
		{
			auto task = _specification
				.createNewTask(_configuration, TaskAffinity(std::vector<const ProcessorNode*>{ processor }));

			if (_specification.acceptsInput())
			{
				if (newData.size() <= i) break;	// Not enough chunks. We need to handle that
				auto& data = newData[i];
				task->setInput(TaskInput(*data));
			}

			task->prepare();
			_createdThreads.emplace_back(*task,
				_wakeNotifier,
				_mutex,
				_threadsShouldWake,
				processor->getOsIndex());
			_createdThreads.back().start();
			i++;
		}
	}

	TaskOutput MultiThreadTask::finalizeAndGetOutputData()
	{
		auto& adapter = _taskBuilder.getDataAdapter();

		std::vector<const RawData*> accumulatedOutputs;
		for (auto& thread: _createdThreads)
		{
			thread.getTaskToRun().finalize();
			accumulatedOutputs.push_back(&thread.getTaskToRun().getOutput().getTaskData());
		}

		return TaskOutput(*adapter.mergeIntoSingleChunk(accumulatedOutputs));
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