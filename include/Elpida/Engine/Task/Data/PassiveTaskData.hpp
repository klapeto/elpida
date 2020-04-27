//
// Created by klapeto on 26/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_TASK_DATA_PASSIVETASKDATA_HPP
#define INCLUDE_ELPIDA_ENGINE_TASK_DATA_PASSIVETASKDATA_HPP

#include "TaskData.hpp"

namespace Elpida
{
	class PassiveTaskData : public TaskData
	{
	public:

		[[nodiscard]] pData getData() const override
		{
			return _data;
		}
		[[nodiscard]] size_t getSize() const override
		{
			return _size;
		}

		PassiveTaskData(pData data, size_t size)
			: _data(data), _size(size)
		{

		}
		~PassiveTaskData() override = default;
	private:
		pData _data;
		size_t _size;
	};
}


#endif //INCLUDE_ELPIDA_ENGINE_TASK_DATA_PASSIVETASKDATA_HPP
