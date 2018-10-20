/*
 * MultiThreadMemoryRead.cpp
 *
 *  Created on: 20 Οκτ 2018
 *      Author: klapeto
 */

#include "TaskBatches/Memory/MultiThreadMemoryRead.hpp"
#include "MemoryRead.hpp"
#include <Elpida/Types/SizedStruct.hpp>
#include <Elpida/CpuInfo.hpp>

namespace Elpida
{
	MultiThreadMemoryRead::MultiThreadMemoryRead(Size size)
			: MultiThreadTask("Memory Read", true), _result("Memory Read Rate", "Bytes"), _size(size)
	{
	}

	MultiThreadMemoryRead::~MultiThreadMemoryRead()
	{
	}

	void MultiThreadMemoryRead::calculateResults()
	{
		addResult(_result);
	}

	void MultiThreadMemoryRead::createTasks()
	{
		auto cores = CpuInfo::getCpuInfo().getLogicalProcessors();
		for (int i = 0; i < cores; ++i)
		{
			//addTask(new MemoryRead<SizedStruct<64>>(_size));
		}
		_result.setMeasuredValue(cores * sizeof(SizedStruct<64> ) * _size);
	}

} /* namespace Elpida */

