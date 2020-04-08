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

#include "Elpida/CommonTasks/AllocateMemory.hpp"

#include "Elpida/Exceptions/ElpidaException.hpp"
#include "Elpida/Topology/SystemTopology.hpp"
#include "Elpida/Topology/ProcessorNode.hpp"
#include "Elpida/CommonTasks/AlignedMemory.hpp"
#include "Elpida/CommonTasks/NumaMemory.hpp"

#include <cstring>

namespace Elpida
{

	AllocateMemory::AllocateMemory(std::size_t size, bool initialize, int numaNode)
		:
		Task("Allocate Memory"),
		_result("Allocation Rate", "Bytes"),
		_size(size),
		_numaNode(numaNode),
		_initialize(initialize),
		_respectNumaAffinity(false)
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

	AllocateMemory::AllocateMemory(std::size_t size, int processorAffinity, bool initialize)
		:
		Task("Allocate Memory"),
		_result("Allocation Rate", "Bytes"),
		_size(size),
		_initialize(initialize),
		_respectNumaAffinity(false)
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

	AllocateMemory::AllocateMemory(std::size_t size, bool initialize, bool respectNumaAffinity)
		:
		Task("Allocate Memory"),
		_result("Allocation Rate", "Bytes"),
		_memory(nullptr),
		_size(size),
		_numaNode(-1),
		_initialize(initialize),
		_respectNumaAffinity(respectNumaAffinity)
	{
		_memory = new NumaMemory(_size, 0);
	}

	void AllocateMemory::applyAffinity()
	{
		if (_respectNumaAffinity)
		{
			_numaNode =
				SystemTopology::getNumaNodeOfProcessor((int)(*_affinity.getProcessorNodes().begin())->getOsIndex());
			((NumaMemory*)_memory)->setNode(_numaNode);
			((NumaMemory*)_memory)->setSize(_size);
		}
		Task::applyAffinity();
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

