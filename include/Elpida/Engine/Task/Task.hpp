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
	class ServiceProvider;
	class ExecutionParameters;

	class Task
	{
	public:

		void setTaskData(TaskDataDto& taskData)
		{
			_taskData = &taskData;
		}

		[[nodiscard]]
		size_t getIterationsToRun() const
		{
			return _iterationsToRun;
		}

		[[nodiscard]]
		const TaskSpecification& getSpecification() const
		{
			return _specification;
		}

		[[nodiscard]]
		virtual TaskResult execute(const ExecutionParameters& parameters) = 0;

		virtual ~Task() = default;
	protected:
		Task(const TaskSpecification& specification,
			const ProcessorNode& processorToRun,
			const ServiceProvider& serviceProvider,
			size_t iterationsToRun);

		const ProcessorNode& _processorToRun;
		const TaskSpecification& _specification;
		const ServiceProvider& _serviceProvider;

		virtual void prepareImpl();
		virtual std::optional<TaskDataDto> finalizeAndGetOutputData();

		[[nodiscard]]
		virtual double getInputDataSize() const;

		[[nodiscard]]
		virtual double calculateTaskResultValue(const Duration& taskElapsedTime) const = 0;

		[[nodiscard]]
		TaskDataDto& getInput() const;

		[[nodiscard]]
		TaskDataDto propagateInput();

		virtual void preProcessExecutionParameters(ExecutionParameters& parameters);

		template<typename TCallable>
		void executeSafe(TCallable callable)
		{
			executeSafe<bool>([&callable]
			{
				callable();
				return true;
			});
		}

		template<typename T, typename TCallable>
		T executeSafe(TCallable callable)
		{
			T returnValue;
			prepare();
			try
			{
				returnValue = std::move(callable());
			}
			catch (...)
			{
				finalize();
				throw;
			}
			finalize();

			return returnValue;
		}

		void prepare();
		void finalize();
	private:
		TaskDataDto* _taskData;
		size_t _iterationsToRun;

		friend class TaskThread;
		friend class MultiThreadTask;
	};
}

#endif //INCLUDE_ELPIDA_ENGINE_TASK_TASK_HPP
