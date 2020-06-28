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

namespace Elpida
{
	class TaskSpecification;
	class ProcessorNode;

	class Task
	{
	public:

		void setInput(const TaskDataDto& input)
		{
			_inputData = input;
		}

		const TaskDataDto& getOutput() const
		{
			return _outputData;
		}

		[[nodiscard]] inline const TaskDataDto& getInput() const
		{
			return _inputData;
		}

		[[nodiscard]] const TaskSpecification& getSpecification() const
		{
			return _specification;
		}

		const ProcessorNode& getProcessorToRun() const
		{
			return _processorToRun;
		}

		void prepare();
		void finalize();
		virtual void execute() = 0;

		[[nodiscard]] TaskResult calculateTaskResult(const Duration& taskElapsedTime) const;

		virtual void applyAffinity();

		Task(const TaskSpecification& specification, const ProcessorNode& processorToRun);
		virtual ~Task() = default;
	protected:
		const ProcessorNode& _processorToRun;
		const TaskSpecification& _specification;

		virtual void prepareImpl() = 0;
		virtual TaskDataDto finalizeAndGetOutputData() = 0;
		[[nodiscard]] virtual double calculateTaskResultValue(const Duration& taskElapsedTime) const = 0;
	private:
		TaskDataDto _inputData;
		TaskDataDto _outputData;

		friend class MultiThreadTask;
	};
}

#endif //INCLUDE_ELPIDA_ENGINE_TASK_TASK_HPP
