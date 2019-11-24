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

#include "TaskBatches/Memory/Read/Cached/MemoryReadCachedTaskBatch.hpp"

#include <initializer_list>

#include "Elpida/Task.hpp"
#include "Elpida/TaskAffinity.hpp"
#include "Elpida/Topology/SystemTopology.hpp"
#include "TaskBatches/General/AllocateMemory.hpp"
#include "TaskBatches/General/Memory.hpp"
#include "TaskBatches/Memory/Read/Cached/MemoryReadCached.hpp"
#include "TaskBatches/Memory/WorkingSetSizes.hpp"

namespace Elpida
{
	void MemoryReadCachedTaskBatch::onBeforeExecution() const
	{
		//TaskThread::setCurrentThreadAffinity(1);
	}

	void MemoryReadCachedTaskBatch::addMemoryReadTask(std::size_t size) const
	{
		auto affinity = TaskAffinity { 1 };
		auto memory = new AllocateMemory(size, true, SystemTopology::getNumaNodeOfProcessor(1));
		memory->setToBeMeasured(false);
		memory->setAffinity(affinity);
		addTask(memory);

		auto read = new MemoryReadCached<>(memory->getMemory());
		read->setAffinity(affinity);
		addTask(read);
	}

	void MemoryReadCachedTaskBatch::createTasks() const
	{
		const auto sizes = {
//				WorkingSetSizes::B_4,
//				WorkingSetSizes::B_8,
//				WorkingSetSizes::B_16,
//				WorkingSetSizes::B_32,
//				WorkingSetSizes::B_64,
//				WorkingSetSizes::B_128,
				WorkingSetSizes::B_256,
				WorkingSetSizes::B_512,
				WorkingSetSizes::KiB_1,
				WorkingSetSizes::KiB_2,
				WorkingSetSizes::KiB_4,
				WorkingSetSizes::KiB_8,
				WorkingSetSizes::KiB_16,
				WorkingSetSizes::KiB_32,
				WorkingSetSizes::KiB_64,
				WorkingSetSizes::KiB_128,
				WorkingSetSizes::KiB_256,
				WorkingSetSizes::KiB_512,
				WorkingSetSizes::MiB_1,
				WorkingSetSizes::MiB_2,
				WorkingSetSizes::MiB_4,
				WorkingSetSizes::MiB_8,
				WorkingSetSizes::MiB_16,
				WorkingSetSizes::MiB_32,
				WorkingSetSizes::MiB_64,
				WorkingSetSizes::MiB_128,
				WorkingSetSizes::MiB_256,
				WorkingSetSizes::MiB_512,
		};

		for (auto size : sizes)
		{
			addMemoryReadTask(size);
		}
	}

} /* namespace Elpida */
