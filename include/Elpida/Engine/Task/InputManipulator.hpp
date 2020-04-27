//
// Created by klapeto on 27/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_TASK_INPUTMANIPULATOR_HPP
#define INCLUDE_ELPIDA_ENGINE_TASK_INPUTMANIPULATOR_HPP

#include "TaskInput.hpp"
namespace Elpida {
	class TaskOutput;
	class TaskAffinity;

	class InputManipulator
	{
	public:

		static TaskInput getUnifiedInput(const TaskOutput& output, const TaskAffinity& affinity);
		static TaskInput getChunkedInput(const TaskOutput& output, const TaskAffinity& affinity);

		InputManipulator() = delete;
		~InputManipulator() = delete;

	private:
		static size_t getAccumulatedSizeOfChunks(const std::vector<TaskData*>& outputChunks);
	};
}


#endif //INCLUDE_ELPIDA_ENGINE_TASK_INPUTMANIPULATOR_HPP
