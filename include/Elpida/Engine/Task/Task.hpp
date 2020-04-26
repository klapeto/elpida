//
// Created by klapeto on 19/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_TASK_TASK_HPP
#define INCLUDE_ELPIDA_ENGINE_TASK_TASK_HPP

#include <string>
#include <utility>
#include "TaskData.hpp"
#include "TaskAffinity.hpp"

namespace Elpida
{

	class TaskData;
	class TaskSpecification;

	class Task
	{
	public:

		void setInput(const TaskData& input)
		{
			_inputData = &input;
		}

		const TaskData& getOutput()
		{
			return _outputData;
		};

		[[nodiscard]] inline const TaskData* getInput() const
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
		virtual TaskData finalizeAndGetOutputData() = 0;
	private:
		TaskData _outputData;
		const TaskData* _inputData;
	};
}

#endif //INCLUDE_ELPIDA_ENGINE_TASK_TASK_HPP
