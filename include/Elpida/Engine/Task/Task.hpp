//
// Created by klapeto on 19/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_TASK_TASK_HPP
#define INCLUDE_ELPIDA_ENGINE_TASK_TASK_HPP

#include <string>
#include <utility>
#include "TaskOutput.hpp"
#include "TaskDataDto.hpp"
#include "Elpida/Engine/Result/TaskResult.hpp"
#include "Elpida/Utilities/Duration.hpp"
#include "TaskAffinity.hpp"

namespace Elpida
{
	class TaskSpecification;

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
