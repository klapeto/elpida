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
#include "Elpida/Utilities/NumaMemory.hpp"
#include "Elpida/Engine/Task/TaskData.hpp"

#include <cstring>
#include <utility>

namespace Elpida
{

	AllocateMemory::AllocateMemory(const TaskSpecification& specification,
		const TaskAffinity& affinity,
		bool toBeCountedOnResults,
		std::size_t size, bool initialize)
		: Task(specification, affinity, toBeCountedOnResults),
		  _size(size),
		  _initialize(initialize)
	{
		_memory = new NumaMemory(size,
			SystemTopology::getNumaNodeOfProcessor((int)affinity.getProcessorNodes().front()->getOsIndex()));
		_outputData = new TaskData(nullptr, size);
	}

	AllocateMemory::~AllocateMemory()
	{
		delete _outputData;
		delete _memory;
	}

	void AllocateMemory::run()
	{
		_memory->allocate();
	}

	void AllocateMemory::prepare()
	{
		if (_memory != nullptr)
		{
			_memory->deallocate();
		}
	}

	void AllocateMemory::finalize()
	{
		if (_memory == nullptr || _memory->getPointer() == nullptr)
		{
			throw ElpidaException("Allocate Memory", "Memory Allocation failed");
		}
		if (_initialize)
		{
			memset(_memory->getPointer(), 0, _memory->getSize());
		}
		_outputData->setData(static_cast<unsigned char*>(_memory->getPointer()));
	}

} /* namespace Elpida */

