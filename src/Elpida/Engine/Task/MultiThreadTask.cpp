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
#include "Elpida/Engine/ServiceProvider.hpp"
#include "Elpida/Config.hpp"
#include "Elpida/Engine/Task/TaskSpecification.hpp"
#include "Elpida/Engine/Task/TaskBuilder.hpp"
#include "Elpida/Engine/Data/DataAdapter.hpp"
#include "Elpida/Engine/Data/DataTransformer.hpp"
#include "Elpida/Engine/Data/RawTaskData.hpp"
#include "Elpida/Engine/Runner/DefaultTaskRunner.hpp"
#include "Elpida/SystemInfo/TimingInfo.hpp"

#include <unordered_set>

namespace Elpida
{

	MultiThreadTask::MultiThreadTask(const TaskBuilder& taskBuilder,
		const TaskConfiguration& configuration,
		const TaskAffinity& affinity,
		const ServiceProvider& serviceProvider,
		size_t iterationsToRun)
		: WorkloadTask(taskBuilder.getTaskSpecification(),
		*affinity.getProcessorNodes().front(),
		serviceProvider,
		iterationsToRun),
		  _affinity(affinity),
		  _configuration(configuration),
		  _serviceProvider(serviceProvider),
		  _originalExecutionParameters(nullptr),
		  _originalDataSize(0),
		  _threadsShouldWake(false)
	{

	}

	TaskResult MultiThreadTask::execute(const ExecutionParameters& parameters)
	{
		_originalExecutionParameters = &parameters;
		prepare();
		run();

		auto metrics = calculateAverageTaskMetrics();

		// divide duration by the task number we had
		Duration finalDuration = metrics.getDuration() / _createdThreads.size();

		finalize();

		return TaskResult(_specification,
			TaskMetrics(finalDuration, metrics.getResultValue(), metrics.getInputDataSize()));
	}

	void MultiThreadTask::prepareImpl()
	{
		const auto& processors = _affinity.getProcessorNodes();
		const size_t processorCount = processors.size();

		_createdThreads.reserve(processorCount);

		if (_specification.acceptsInput() || _specification.producesOutput())
		{
			// Complex tasks
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

			size_t i = 0;
			for (auto processor : processors)
			{
				TaskDataDto curTaskInput;

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


				auto& thread = _createdThreads.emplace_back(
					_specification.createNewTask(_configuration, *processor, _serviceProvider, 1),
					std::move(curTaskInput),
					_wakeNotifier,
					_mutex,
					_serviceProvider.getTimingInfo(),
					*_originalExecutionParameters,
					_threadsShouldWake,
					processor->getOsIndex());

				thread.start();
				i++;
			}
		}
		else
		{
			// Microtasks

			bool parametersProcessed = false;
			// Grab a copy of the parameters
			_executionParameters = *_originalExecutionParameters;
			for (auto& processor: processors)
			{
				auto task = _specification.createNewTask(_configuration, *processor, _serviceProvider, 1);

				if (!parametersProcessed)
				{
					task->preProcessExecutionParameters(_executionParameters);
					parametersProcessed = true;
				}

				auto& thread = _createdThreads.emplace_back(
					std::move(task),
					TaskDataDto(),
					_wakeNotifier,
					_mutex,
					_serviceProvider.getTimingInfo(),
					_executionParameters,
					_threadsShouldWake,
					processor->getOsIndex());

				thread.start();
			}
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
			_threadsShouldWake = false;
			return TaskDataDto(std::move(returnData), properties);
		}
		else
		{
			_createdThreads.clear();
			_threadsShouldWake = false;
			return std::nullopt;
		}
	}

	double MultiThreadTask::calculateTaskResultValue(const Duration& taskElapsedTime) const
	{
		return 0.0;
	}

	void MultiThreadTask::run()
	{
		{
			std::unique_lock<std::mutex> lock(_mutex);
			_threadsShouldWake = true;
			_wakeNotifier.notify_all();
		}

		for (auto& thread : _createdThreads)
		{
			thread.join();
		}
		_threadsShouldWake = false;
	}

	TaskMetrics MultiThreadTask::calculateAverageTaskMetrics() const
	{
		double totalDuration = 0.0;
		double totalValue = 0.0;

		for (auto& thread : _createdThreads)
		{
			auto metrics = thread.getMetrics();

			totalDuration += metrics.getDuration().count();
			totalValue += metrics.getResultValue();
		}

		return TaskMetrics(Duration(
			totalDuration / static_cast<double>(_createdThreads.size())),
			totalValue / static_cast<double>(_createdThreads.size()),
			getInputDataSize());
	}
}