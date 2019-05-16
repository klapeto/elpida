/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2018  Ioannis Panagiotopoulos
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>
 *************************************************************************/

/*
 * MemoryTaskBatch.cpp
 *
 *  Created on: 18 Οκτ 2018
 *      Author: klapeto
 */

#include "TaskBatches/Memory/MemoryReadCachedTaskBatch.hpp"

#include "TaskBatches/General/AllocateMemory.hpp"
#include <Elpida/CpuInfo.hpp>
#include "TaskBatches/Memory/MemoryReadCached.hpp"
#include "TaskBatches/Memory/MultiThreadMemoryChunksRead.hpp"

namespace Elpida
{

	constexpr int MemoryReadCachedTaskBatch::workingSetSize[];

	void MemoryReadCachedTaskBatch::onBeforeExecution() const
	{
		//TaskThread::setCurrentThreadAffinity(1);
	}

	void MemoryReadCachedTaskBatch::addMemoryReadTask(Size size) const
	{
		auto memory = new AllocateMemory(size, true, sizeof(void*) * 16);
		memory->setToBeMeasured(false);
		addTask(memory);
		addTask(new MemoryReadCached(memory->getMemory(), std::chrono::milliseconds(2000)));
	}

	void MemoryReadCachedTaskBatch::createTasks() const
	{
		for (auto size : workingSetSize)
		{
			addMemoryReadTask(size);
		}
	}

} /* namespace Elpida */
