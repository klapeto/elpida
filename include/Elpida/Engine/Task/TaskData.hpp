//
// Created by klapeto on 19/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_TASK_TASKDATA_HPP
#define INCLUDE_ELPIDA_ENGINE_TASK_TASKDATA_HPP

#include <cstddef>
namespace Elpida
{
	class TaskData final
	{
	public:
		using Data = unsigned char;
		using pData = Data*;

		[[nodiscard]] pData getData() const
		{
			return _data;
		}
		[[nodiscard]] size_t getSize() const
		{
			return _size;
		}

		TaskData(pData data, size_t size)
			: _data(data), _size(size)
		{
		}

		TaskData()
			: _data(nullptr), _size(0)
		{
		}

		TaskData(const TaskData&) = delete;
		TaskData& operator=(const TaskData&) = delete;

		TaskData(TaskData&&) = default;
		TaskData& operator=(TaskData&&) = default;

		~TaskData() = default;
	private:
		pData _data;
		size_t _size;
	};
}


#endif //INCLUDE_ELPIDA_ENGINE_TASK_TASKDATA_HPP
