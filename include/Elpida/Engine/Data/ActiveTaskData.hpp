//
// Created by klapeto on 26/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_TASK_DATA_ACTIVETASKDATA_HPP
#define INCLUDE_ELPIDA_ENGINE_TASK_DATA_ACTIVETASKDATA_HPP


#include "Elpida/Utilities/RawData.hpp"
#include "Elpida/Utilities/NumaMemory.hpp"

namespace Elpida
{
	class ActiveTaskData : public RawData
	{
	public:

		[[nodiscard]] pData getData() const override
		{
			return _memory.getPointer();
		}
		[[nodiscard]] size_t getSize() const override
		{
			return _memory.getSize();
		}

		ActiveTaskData(size_t size, int numaNode)
			: _memory(size, numaNode)
		{
			_memory.allocate();
		}
		~ActiveTaskData() override = default;
	private:
		NumaMemory _memory;
	};
}


#endif //INCLUDE_ELPIDA_ENGINE_TASK_DATA_ACTIVETASKDATA_HPP
