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
// Created by klapeto on 19/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_TASK_TASK_HPP
#define INCLUDE_ELPIDA_ENGINE_TASK_TASK_HPP

#include <string>
#include <utility>
#include "TaskDataDto.hpp"
#include "Elpida/Engine/Result/TaskResult.hpp"
#include "Elpida/Utilities/Duration.hpp"
#include "Elpida/Utilities/OptionalReference.hpp"

namespace Elpida
{
	class TaskSpecification;
	class ProcessorNode;

	class Task
	{
	public:

		void setTaskData(TaskDataDto& taskData)
		{
			_taskData = &taskData;
		}

		[[nodiscard]] size_t getIterationsToRun() const
		{
			return _iterationsToRun;
		}

		[[nodiscard]] const TaskSpecification& getSpecification() const
		{
			return _specification;
		}

		[[nodiscard]] const ProcessorNode& getProcessorToRun() const
		{
			return _processorToRun;
		}

		void prepare();
		virtual void execute() = 0;
		void finalize();

		[[nodiscard]] TaskResult calculateTaskResult(const Duration& taskElapsedTime) const;

		virtual void applyAffinity();

		virtual ~Task() = default;
	protected:
		Task(const TaskSpecification& specification, const ProcessorNode& processorToRun, size_t iterationsToRun);

		const ProcessorNode& _processorToRun;
		const TaskSpecification& _specification;

		virtual void prepareImpl()
		{
		}
		virtual std::optional<TaskDataDto> finalizeAndGetOutputData()
		{
			return std::nullopt;
		}
		[[nodiscard]] virtual double calculateTaskResultValue(const Duration& taskElapsedTime) const = 0;

		[[nodiscard]] TaskDataDto& getInput() const
		{
			return *_taskData;
		}

		TaskDataDto propagateInput()
		{
			return TaskDataDto(std::move(*_taskData));
		}
	private:
		TaskDataDto* _taskData;
		size_t _iterationsToRun;

		friend class TaskThread;
		friend class MultiThreadTask;
	};
}

#endif //INCLUDE_ELPIDA_ENGINE_TASK_TASK_HPP
