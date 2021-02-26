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

#ifndef INCLUDE_ELPIDA_ENGINE_TASK_MULTITHREADTASK_HPP
#define INCLUDE_ELPIDA_ENGINE_TASK_MULTITHREADTASK_HPP

#include "WorkloadTask.hpp"
#include "TaskThread.hpp"
#include "TaskAffinity.hpp"
#include "Elpida/Engine/Task/ExecutionParameters.hpp"

namespace Elpida
{
	class TaskConfiguration;
	class TaskBuilder;
	class ServiceProvider;

	class MultiThreadTask : public WorkloadTask
	{
	public:
		TaskResult execute(const ExecutionParameters& parameters) override;

		MultiThreadTask(const TaskBuilder& taskBuilder,
			const TaskConfiguration& configuration,
			const TaskAffinity& affinity,
			const ServiceProvider& serviceProvider,
			size_t iterationsToRun);
		~MultiThreadTask() override = default;
	protected:
		void run() override;
		void prepareImpl() override;
		std::optional<TaskDataDto> finalizeAndGetOutputData() override;
		[[nodiscard]] double calculateTaskResultValue(const Duration& taskElapsedTime) const override;
	private:
		std::mutex _mutex;
		std::condition_variable _wakeNotifier;
		std::vector<TaskThread> _createdThreads;
		TaskAffinity _affinity;
		ExecutionParameters _executionParameters;
		const TaskConfiguration& _configuration;
		const ServiceProvider& _serviceProvider;
		const ExecutionParameters* _originalExecutionParameters;
		size_t _originalDataSize;
		bool _threadsShouldWake;

		[[nodiscard]] TaskMetrics calculateAverageTaskMetrics() const;
	};
}


#endif //INCLUDE_ELPIDA_ENGINE_TASK_MULTITHREADTASK_HPP
