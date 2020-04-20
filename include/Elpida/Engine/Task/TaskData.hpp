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
		[[nodiscard]] void* getData() const
		{
			return _data;
		}
		[[nodiscard]] size_t getSize() const
		{
			return _size;
		}

		void setData(void* data)
		{
			_data = data;
		}

		void setSize(size_t size)
		{
			_size = size;
		}

		TaskData(void* data, size_t size)
			: _data(data), _size(size)
		{
		}
		~TaskData() = default;
	private:
		void* _data;
		size_t _size;
	};
}


#endif //INCLUDE_ELPIDA_ENGINE_TASK_TASKDATA_HPP
