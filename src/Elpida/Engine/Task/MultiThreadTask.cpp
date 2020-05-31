//
// Created by klapeto on 20/4/20.
//

#include <cmath>
#include "Elpida/Engine/Task/MultiThreadTask.hpp"
#include "Elpida/Topology/ProcessorNode.hpp"
#include "Elpida/Config.hpp"
#include "Elpida/Engine/Task/TaskSpecification.hpp"
#include "Elpida/Engine/Task/TaskBuilder.hpp"
#include "Elpida/Engine/Data/DataAdapter.hpp"
#include "Elpida/Engine/Data/DataPropertiesTransformer.hpp"

#include <unordered_set>

namespace Elpida
{

	MultiThreadTask::MultiThreadTask(const TaskBuilder& taskBuilder,
		const TaskConfiguration& configuration,
		const TaskAffinity& affinity)
		: Task(taskBuilder.getTaskSpecification(), *affinity.getProcessorNodes().front()),
		  _affinity(affinity),
		  _configuration(configuration),
		  _taskBuilder(taskBuilder),
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
		const auto& processors = _affinity.getProcessorNodes();
		const size_t processorCount = processors.size();

		auto input = getInput();

		_allocatedChunks =
			DataAdapter::breakIntoChunks(*input.getTaskData(), _affinity, _specification.getInputDataSpecification());

		_createdThreads.reserve(processorCount);

		auto dataPropertiesTransformer = _specification.getDataPropertiesTransformer();

		size_t i = 0;
		for (auto processor : processors)
		{
			auto task = _specification.createNewTask(_configuration, *processor);

			if (_specification.acceptsInput())
			{
				if (_allocatedChunks.size() < i)
				{
					delete task;
					break;    // Not enough chunks. We need to handle that
				}
				auto& data = _allocatedChunks[i];
				if (dataPropertiesTransformer == nullptr)
				{
					task->setInput(TaskDataDto(*data, input.getDefinedProperties()));
				}
				else
				{
					task->setInput(TaskDataDto(*data,
						dataPropertiesTransformer->transform(input.getTaskData()->getSize(),
							input.getDefinedProperties(),
							data->getSize())));
				}
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

	TaskDataDto MultiThreadTask::finalizeAndGetOutputData()
	{
		std::unordered_set<const RawData*> accumulatedOutputs;
		for (auto& thread: _createdThreads)
		{
			thread.getTaskToRun().finalize();
			accumulatedOutputs.emplace(thread.getTaskToRun().getOutput().getTaskData());
		}

		auto returnData = DataAdapter::mergeIntoSingleChunk(std::vector<const RawData*>(accumulatedOutputs.begin(),
			accumulatedOutputs.end()));

		// This becomes the delete 'list'
		for (auto data: _allocatedChunks)
		{
			// We need to add the original chunks to delete list too if tasks created their own chunks
			accumulatedOutputs.emplace(data);
		}

		for (auto data: accumulatedOutputs)
		{
			delete data;    // delete any original chunks and chunks created by the tasks
		}
		return TaskDataDto(*returnData, getInput().getDefinedProperties());
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