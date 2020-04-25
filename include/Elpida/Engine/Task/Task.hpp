//
// Created by klapeto on 19/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_TASK_TASK_HPP
#define INCLUDE_ELPIDA_ENGINE_TASK_TASK_HPP

#include <string>
#include <utility>
#include "TaskSpecification.hpp"

namespace Elpida
{

	class TaskData;

	class Task
	{
	public:

		virtual void setInput(TaskData* const* input)
		{
			_inputData = input;
		};
		virtual TaskData* const* getOutput()
		{
			return &_outputData;
		};

		[[nodiscard]] virtual const TaskData* getInput() const
		{
			return _inputData != nullptr ? *_inputData : nullptr;
		};

		[[nodiscard]] bool shouldBeCountedOnResults() const
		{
			return _toBeCountedOnResults;
		}

		[[nodiscard]] const TaskSpecification& getSpecification() const
		{
			return _specification;
		}

		virtual void prepare() = 0;
		virtual void finalize() = 0;
		virtual void run() = 0;

		virtual void applyAffinity();

		Task(const TaskSpecification& specification, TaskAffinity  affinity, bool toBeCountedOnResults = true)
			: _specification(specification), _affinity(std::move(affinity)), _inputData(nullptr), _outputData(nullptr),
			  _toBeCountedOnResults(toBeCountedOnResults)
		{
		}

		virtual ~Task() = default;
	protected:
		TaskAffinity _affinity;
		const TaskSpecification& _specification;
		TaskData* const* _inputData;
		TaskData* _outputData;
		bool _toBeCountedOnResults;
	};
}

#endif //INCLUDE_ELPIDA_ENGINE_TASK_TASK_HPP
