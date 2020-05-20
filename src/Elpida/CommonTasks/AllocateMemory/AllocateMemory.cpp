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
 * AllocateMemory.cpp
 *
 *  Created on: 20 Οκτ 2018
 *      Author: klapeto
 */

#include "Elpida/CommonTasks/AllocateMemory/AllocateMemory.hpp"

#include "Elpida/ElpidaException.hpp"
#include "Elpida/Topology/SystemTopology.hpp"
#include "Elpida/Topology/ProcessorNode.hpp"
#include "Elpida/Engine/Data/PassiveTaskData.hpp"

#include <cstring>

namespace Elpida
{

	AllocateMemory::AllocateMemory(const TaskSpecification& specification,
		const TaskAffinity& affinity,
		std::size_t size, bool initialize)
		: Task(specification, affinity),
		  _memory(size,
			  SystemTopology::getNumaNodeOfProcessor((int)affinity.getProcessorNodes().front()->getOsIndex())),
		  _size(size),
		  _initialize(initialize)
	{

	}
	void AllocateMemory::execute()
	{
		_memory.allocate();
	}

	void AllocateMemory::prepareImpl()
	{
		_memory.deallocate();
	}

	TaskOutput AllocateMemory::finalizeAndGetOutputData()
	{
		if (_memory.getPointer() == nullptr)
		{
			throw ElpidaException("Allocate Memory", "Memory Allocation failed");
		}
		if (_initialize)
		{
			memset(_memory.getPointer(), 0, _memory.getSize());
		}
		return TaskOutput(new PassiveTaskData( _memory.getPointer(), _size ));
	}

	double AllocateMemory::calculateTaskResultValue(const Duration& taskElapsedTime) const
	{
		return _size;
	}

} /* namespace Elpida */

