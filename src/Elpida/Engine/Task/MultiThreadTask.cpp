/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2020  Ioannis Panagiotopoulos
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>
 *************************************************************************/

//
// Created by klapeto on 20/4/20.
//

#include "Elpida/Engine/Task/MultiThreadTask.hpp"

#include "Elpida/SystemInfo/ProcessorNode.hpp"
#include "Elpida/Config.hpp"
#include "Elpida/Engine/Task/TaskSpecification.hpp"
#include "Elpida/Engine/Task/TaskBuilder.hpp"
#include "Elpida/Engine/Data/DataAdapter.hpp"
#include "Elpida/Engine/Data/DataPropertiesTransformer.hpp"
#include "Elpida/Utilities/RawData.hpp"

#include <unordered_set>

namespace Elpida
{

	MultiThreadTask::MultiThreadTask(const TaskBuilder& taskBuilder,
		const TaskConfiguration& configuration,
		const TaskAffinity& affinity, size_t iterationsToRun)
		: Task(taskBuilder.getTaskSpecification(), *affinity.getProcessorNodes().front(), iterationsToRun),
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
			auto task = _specification.createNewTask(_configuration, *processor, 1);

			if (_specification.acceptsInput())
			{
				if (_allocatedChunks.size() <= i)	// TODO: Move it up to avoid task creation
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
		std::unordered_set<const RawData*> outputsSet;
		std::vector<const RawData*> accumulatedOutputs;
		for (auto& thread: _createdThreads)
		{
			thread.getTaskToRun().finalize();
			accumulatedOutputs.push_back(thread.getTaskToRun().getOutput().getTaskData());
			outputsSet.emplace(thread.getTaskToRun().getOutput().getTaskData());
		}

		auto returnData = DataAdapter::mergeIntoSingleChunk(accumulatedOutputs, *_affinity.getProcessorNodes().front());

		// This becomes the delete 'list'
		for (auto data: _allocatedChunks)
		{
			// We need to add the original chunks to delete list too if tasks created their own chunks
			outputsSet.emplace(data);
		}

		for (auto data: outputsSet)
		{
			delete data;    // delete any original chunks and chunks created by the tasks
		}

		_createdThreads.clear();
		_allocatedChunks.clear();

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