//
// Created by klapeto on 19/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_TASK_TASK_HPP
#define INCLUDE_ELPIDA_ENGINE_TASK_TASK_HPP

#include <string>
#include <utility>
#include "TaskOutput.hpp"
#include "TaskInput.hpp"
#include "TaskAffinity.hpp"

namespace Elpida
{

	class TaskSpecification;

	class Task
	{
	public:

		void setInput(const TaskInput& input)
		{
			_inputData = input;
		}

		void setInput(TaskInput&& input)
		{
			_inputData = input;
		}

		TaskOutput& getOutput()
		{
			return _outputData;
		};

		[[nodiscard]] inline const TaskInput& getInput() const
		{
			return _inputData;
		};

		[[nodiscard]] bool shouldBeCountedOnResults() const
		{
			return _toBeCountedOnResults;
		}

		[[nodiscard]] const TaskSpecification& getSpecification() const
		{
			return _specification;
		}

		void prepare();
		void finalize();
		virtual void execute() = 0;

		virtual void applyAffinity();

		Task(const TaskSpecification& specification, TaskAffinity affinity, bool toBeCountedOnResults = true);

		virtual ~Task() = default;
	protected:
		TaskAffinity _affinity;
		const TaskSpecification& _specification;
		bool _toBeCountedOnResults;

		virtual void prepareImpl() = 0;
		virtual TaskOutput finalizeAndGetOutputData() = 0;
	private:
		TaskOutput _outputData;
		TaskInput _inputData;
	};
}

#endif //INCLUDE_ELPIDA_ENGINE_TASK_TASK_HPP
