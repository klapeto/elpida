//
// Created by klapeto on 19/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_TASK_TASKDATA_HPP
#define INCLUDE_ELPIDA_ENGINE_TASK_TASKDATA_HPP

#include <cstddef>

namespace Elpida
{
	class TaskData
	{
	public:
		using Data = unsigned char;
		using pData = Data*;

		[[nodiscard]] virtual pData getData() const = 0;
		[[nodiscard]] virtual size_t getSize() const = 0;

		TaskData() = default;
		TaskData(const TaskData&) = delete;
		TaskData& operator=(const TaskData&) = delete;
		TaskData(TaskData&&) = default;
		TaskData& operator=(TaskData&&) = default;
		virtual ~TaskData() = default;
	};
}


#endif //INCLUDE_ELPIDA_ENGINE_TASK_TASKDATA_HPP
