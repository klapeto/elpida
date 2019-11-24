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

#include "TaskBatches/General/AllocateMemory.hpp"
#include <Elpida/Config.hpp>
#include <Elpida/Exceptions/ElpidaException.hpp>
#include <Elpida/Topology/SystemTopology.hpp>
#include "AlignedMemory.hpp"
#include "NumaMemory.hpp"
#include <cstdlib>
#include <cstring>

namespace Elpida
{

	AllocateMemory::AllocateMemory(Size size, bool initialize, int numaNode)
			: Task("Allocate Memory"), _result("Allocation Rate", "Bytes"), _numaNode(numaNode), _initialize(initialize)
	{
		if (_numaNode == -1)
		{
			_memory = new AlignedMemory(size, sizeof(void*) * 16);
		}
		else
		{
			_memory = new NumaMemory(size, _numaNode);
		}
	}

	AllocateMemory::AllocateMemory(Size size, int processorAffinity, bool initialize)
			: Task("Allocate Memory"), _result("Allocation Rate", "Bytes"), _initialize(initialize)
	{
		if (processorAffinity < 0)
		{
			_numaNode = -1;
			_memory = new AlignedMemory(size, sizeof(void*) * 16);
		}
		else
		{
			_numaNode = SystemTopology::getNumaNodeOfProcessor(processorAffinity);
			_memory = new NumaMemory(size, _numaNode);
		}
	}

	AllocateMemory::~AllocateMemory()
	{
		if (_memory != nullptr)
		{
			delete _memory;
		}
	}

	void AllocateMemory::run()
	{
		_memory->allocate();
	}

	void AllocateMemory::calculateResults(const TaskMetrics& metrics)
	{
		if (_memory != nullptr)
		{
			_result = _memory->getSize();
		}
		addResult(_result);
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
			throw ElpidaException("Memory Allocation failed");
		}
		if (_initialize)
		{
			memset(_memory->getPointer(), 0, _memory->getSize());
		}
	}

} /* namespace Elpida */

