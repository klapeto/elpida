/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2020  Ioannis Panagiotopoulos
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

#include "Elpida/Engine/Data/ActiveTaskData.hpp"

#include <cstring>

namespace Elpida
{

	AllocateMemory::AllocateMemory(const TaskSpecification& specification,
		const ProcessorNode& processorToRun,
		std::size_t size,
		size_t iterationsToRun)
		: Task(specification, processorToRun, iterationsToRun),
		  _memory(nullptr),
		  _size(size)
	{

	}
	void AllocateMemory::execute()
	{
		_memory = new ActiveTaskData(_size, _processorToRun);
		memset(_memory->getData(), 0, _memory->getSize());
	}

	void AllocateMemory::prepareImpl()
	{

	}

	TaskDataDto AllocateMemory::finalizeAndGetOutputData()
	{
		return TaskDataDto(*_memory);
	}

	double AllocateMemory::calculateTaskResultValue(const Duration& taskElapsedTime) const
	{
		return _size;
	}

} /* namespace Elpida */

