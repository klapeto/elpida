//
// Created by klapeto on 27/2/2023.
//

#include "TaskData.hpp"

namespace Elpida
{

	void TaskData::Allocate(std::size_t size)
	{
		_data = NumaUniquePtr(_numaNodeId, size);
	}

	TaskData::TaskData(long numaNodeId)
		: _numaNodeId(numaNodeId), _size(0)
	{
	}
} // Elpida