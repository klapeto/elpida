//
// Created by klapeto on 10/5/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_TASK_DATA_DATAADAPTER_HPP
#define INCLUDE_ELPIDA_ENGINE_TASK_DATA_DATAADAPTER_HPP

#include "Elpida/Engine/Task/TaskInput.hpp"

namespace Elpida
{
	class TaskOutput;
	class TaskAffinity;

	class DataAdapter
	{
	public:
		[[nodiscard]] virtual TaskInput transformOutputToInput(const TaskOutput& output, const TaskAffinity& affinity) const = 0;
		virtual ~DataAdapter() = default;
	protected:
		static size_t getAccumulatedSizeOfChunks(const std::vector<TaskData*>& outputChunks);
	};
}

#endif //INCLUDE_ELPIDA_ENGINE_TASK_DATA_DATAADAPTER_HPP
