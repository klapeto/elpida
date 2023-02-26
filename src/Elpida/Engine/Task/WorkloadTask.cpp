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

#include "Elpida/Engine/Task/WorkloadTask.hpp"
#include "Elpida/Engine/ServiceProvider.hpp"
#include "Elpida/SystemInfo/TimingInfo.hpp"
#include "Elpida/Engine/Runner/DefaultTaskRunner.hpp"

namespace Elpida
{

	TaskResult WorkloadTask::execute(const ExecutionParameters& parameters)
	{
		Duration taskDuration;
		double value = 0.0;

		executeSafe([this, &taskDuration, &value]
		{
			auto& timingInfo = _serviceProvider.getTimingInfo();

			taskDuration = DefaultTaskRunner::measure(
				timingInfo.getNowOverhead(),
				[this]
				{
					run();
				});
			value = calculateTaskResultValue(taskDuration);
		});

		return TaskResult(_specification, TaskMetrics(taskDuration, value, getInputDataSize()));
	}

	WorkloadTask::WorkloadTask(const TaskSpecification& specification,
		const ProcessorNode& processorToRun,
		const ServiceProvider& serviceProvider,
		size_t iterationsToRun)
		: Task(specification, processorToRun, serviceProvider, iterationsToRun)
	{
	}
}