/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2021  Ioannis Panagiotopoulos
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
// Created by klapeto on 21/2/21.
//

#include "Elpida/Engine/Task/MicroTask.hpp"
#include "Elpida/Engine/ServiceProvider.hpp"
#include "Elpida/SystemInfo/TimingInfo.hpp"
#include "Elpida/Engine/Runner/MicroTaskRunner.hpp"
#include "Elpida/Engine/Task/ExecutionParameters.hpp"

namespace Elpida
{

	MicroTask::MicroTask(const TaskSpecification& specification,
		const ProcessorNode& processorToRun,
		const ServiceProvider& serviceProvider,
		size_t iterationsToRun)
		: Task(specification, processorToRun, serviceProvider, iterationsToRun)
	{
	}

	TaskResult MicroTask::execute(const ExecutionParameters& parameters)
	{
		auto& timingInfo = _serviceProvider.getTimingInfo();
		Duration taskDuration;
		double value = 0.0;

		auto fixedIterations = parameters.getParameter(FixedIterationsName);
		if (fixedIterations)
		{
			executeSafe([this, &taskDuration, &value, &fixedIterations, &timingInfo]
			{
				taskDuration = MicroTaskRunner::measure(
					fixedIterations.value(),
					timingInfo.getNowOverhead(),
					timingInfo.getLoopOverhead(),
					[this](auto iterations)
					{
						run(iterations);
					});

				value = calculateTaskResultValue(taskDuration);
			});
		}
		else
		{
			auto targetTime = std::max(getMinimumExecutionTime(), timingInfo.getTargetTime());

			executeSafe([this, &taskDuration, &value, &targetTime, &timingInfo]
			{
				taskDuration = MicroTaskRunner::measure(
					targetTime,
					getMinimumIterations(),
					timingInfo.getNowOverhead(),
					timingInfo.getLoopOverhead(),
					[this](auto iterations)
					{
						run(iterations);
					});

				value = calculateTaskResultValue(taskDuration);
			});
		}

		return TaskResult(_specification, TaskMetrics(taskDuration, value, getInputDataSize()));
	}

	Duration MicroTask::getMinimumExecutionTime() const
	{
		return _serviceProvider.getTimingInfo().getTargetTime();
	}

	void MicroTask::preProcessExecutionParameters(ExecutionParameters& parameters)
	{
		auto iterations = executeSafe<size_t>([this]()
		{
			auto& timingInfo = _serviceProvider.getTimingInfo();
			auto targetTime = std::max(getMinimumExecutionTime(), timingInfo.getTargetTime());

			return timingInfo.calculateIterationsForTime(targetTime,
				[this](auto iterations)
				{
					run(iterations);
				});
		});

		parameters.setParameter(FixedIterationsName, iterations);
	}

	size_t MicroTask::getMinimumIterations() const
	{
		return 1;
	}
}