//
// Created by klapeto on 26/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_TASK_TASKDATADTO_HPP
#define INCLUDE_ELPIDA_ENGINE_TASK_TASKDATADTO_HPP

#include <unordered_map>

namespace Elpida
{
	class RawData;

	class TaskDataDto final
	{
	public:

		[[nodiscard]] RawData* getTaskData() const
		{
			return _taskData;
		}

		const std::unordered_map<std::string, double>& getDefinedProperties() const
		{
			return _definedProperties;
		}

		TaskDataDto();
		explicit TaskDataDto(RawData& taskData);
		explicit TaskDataDto(RawData& taskData, std::unordered_map<std::string, double> properties);
		TaskDataDto(const TaskDataDto&) = default;
		TaskDataDto& operator=(const TaskDataDto&) = default;
		TaskDataDto(TaskDataDto&& other) noexcept = default;
		TaskDataDto& operator=(TaskDataDto&& other) noexcept = default;
		~TaskDataDto() = default;
	private:
		std::unordered_map<std::string, double> _definedProperties;
		RawData* _taskData;
	};
}


#endif //INCLUDE_ELPIDA_ENGINE_TASK_TASKDATADTO_HPP
