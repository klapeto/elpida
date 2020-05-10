//
// Created by klapeto on 19/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_TASK_TASK_HPP
#define INCLUDE_ELPIDA_ENGINE_TASK_TASK_HPP

#include <string>
#include <utility>
#include "TaskOutput.hpp"
#include "TaskInput.hpp"
#include "Elpida/Engine/Result/TaskResult.hpp"
#include "Elpida/Utilities/Duration.hpp"
#include "TaskAffinity.hpp"

namespace Elpida
{
	class TaskSpecification;

	class Task
	{
	public:

		void setInput(TaskInput&& input)
		{
			_inputData = std::move(input);
		}

		TaskOutput& getOutput()
		{
			return _outputData;
		}

		[[nodiscard]] inline const TaskInput& getInput() const
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

		Task(const TaskSpecification& specification, TaskAffinity affinity);
		virtual ~Task() = default;
	protected:
		TaskAffinity _affinity;
		const TaskSpecification& _specification;

		virtual void prepareImpl() = 0;
		virtual TaskOutput finalizeAndGetOutputData() = 0;
		[[nodiscard]] virtual size_t calculateTaskResultValue(const Duration& taskElapsedTime) const = 0;
	private:
		TaskOutput _outputData;
		TaskInput _inputData;

		friend class MultiThreadTask;
	};
}

#endif //INCLUDE_ELPIDA_ENGINE_TASK_TASK_HPP
