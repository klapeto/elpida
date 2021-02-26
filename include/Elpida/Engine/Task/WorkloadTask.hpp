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

#ifndef INCLUDE_ELPIDA_ENGINE_TASK_WORKLOADTASK_HPP
#define INCLUDE_ELPIDA_ENGINE_TASK_WORKLOADTASK_HPP

#include "Task.hpp"

namespace Elpida
{
	class WorkloadTask : public Task
	{
	public:
		TaskResult execute(const ExecutionParameters& parameters) override;

		~WorkloadTask() override = default;
	protected:
		WorkloadTask(const TaskSpecification& specification,
			const ProcessorNode& processorToRun,
			const ServiceProvider& serviceProvider,
			size_t iterationsToRun);
		virtual void run() = 0;
	};
}

#endif //INCLUDE_ELPIDA_ENGINE_TASK_WORKLOADTASK_HPP
