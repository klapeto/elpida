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
#include "Elpida/Engine/Data/DataTransformer.hpp"
#include "Elpida/Engine/Data/RawTaskData.hpp"

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
		  _originalDataSize(0),
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

		auto& input = *_taskData;

		_originalDataSize = input.getTaskData()->getSize();

		auto dataPropertiesTransformer = _specification.getDataTransformer();

		size_t divisibleBy = 1;

		if (_specification.getInputDataSpecification().divisibleByProperty())
		{
			divisibleBy = input.getDefinedProperties()
				.at(_specification.getInputDataSpecification().getSizeDivisibleByPropertyName());
		}
		else if (_specification.getInputDataSpecification().getSizeShouldBeDivisibleBy() > 0)
		{
			divisibleBy = _specification.getInputDataSpecification().getSizeShouldBeDivisibleBy();
		}

		auto _allocatedChunks = DataAdapter::breakIntoChunks(
			{ input.getTaskData().get() },
			_affinity,
			divisibleBy);

		// Since we will replace the memory, release the input
		// to reduce the memory consumption
		if (_specification.producesOutput())
		{
			input.getTaskDataAndRelease().reset();
		}

		_createdThreads.reserve(processorCount);

		size_t i = 0;
		for (auto processor : processors)
		{
			TaskDataDto curTaskInput;
			if (_specification.acceptsInput())
			{
				if (_allocatedChunks.size() <= i)
				{
					break;    // Not enough chunks. We need to handle that
				}

				curTaskInput.setTaskData(std::move(_allocatedChunks[i]));

				if (!dataPropertiesTransformer)
				{
					curTaskInput.setProperties(input.getDefinedProperties());
				}
				else
				{
					curTaskInput.setProperties(
						dataPropertiesTransformer->transformDataProperties(
							_originalDataSize,
							input.getDefinedProperties(),
							curTaskInput.getTaskData()->getSize(),
							curTaskInput.getTaskData()->getSize() / (float)_originalDataSize));
				}
			}

			auto& thread = _createdThreads.emplace_back(
				_specification.createNewTask(_configuration, *processor, 1),
				std::move(curTaskInput),
				_wakeNotifier,
				_mutex,
				_threadsShouldWake,
				processor->getOsIndex());

			thread.prepare();
			thread.start();
			i++;
		}
	}

	std::optional<TaskDataDto> MultiThreadTask::finalizeAndGetOutputData()
	{
		if (getSpecification().producesOutput())
		{
			std::vector<std::unique_ptr<RawTaskData>> accumulatedOutputs;
			std::vector<const RawTaskData*> accumulatedOutputsPointers;
			accumulatedOutputsPointers.reserve(_createdThreads.size());
			accumulatedOutputs.reserve(_createdThreads.size());

			for (auto& thread: _createdThreads)
			{
				thread.finalize();

				accumulatedOutputs.push_back(thread.getTaskData().getTaskDataAndRelease());
				accumulatedOutputsPointers.push_back(accumulatedOutputs.back().get());
			}

			auto returnData =
				DataAdapter::mergeIntoSingleChunk(accumulatedOutputsPointers, *_affinity.getProcessorNodes().front());

			std::unordered_map<std::string, double> properties;
			auto dataPropertiesTransformer = _specification.getDataTransformer();
			if (dataPropertiesTransformer)
			{
				properties = dataPropertiesTransformer->transformDataProperties(_originalDataSize,
					_taskData->getDefinedProperties(),
					returnData->getSize(),
					1.0);
			}
			else
			{
				properties = _taskData->getDefinedProperties();
			}

			_createdThreads.clear();
			return TaskDataDto(std::move(returnData), properties);
		}
		else
		{
			_createdThreads.clear();
			return std::nullopt;
		}
	}

	double MultiThreadTask::calculateTaskResultValue(const Duration& taskElapsedTime) const
	{
		double accumulator = 0;
		for (auto& thread: _createdThreads)
		{
			accumulator += thread.calculateTaskResultValue(taskElapsedTime);
		}
		return _specification.getResultSpecification().getAggregationType() == ResultSpecification::Accumulative
			   ? accumulator : accumulator / _createdThreads.size();
	}
}