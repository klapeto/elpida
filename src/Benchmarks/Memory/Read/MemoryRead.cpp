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
 * MemoryRead.cpp
 *
 *  Created on: 18 Οκτ 2018
 *      Author: klapeto
 */

#include "Benchmarks/Memory/Read/MemoryRead.hpp"

#include <Elpida/ElpidaException.hpp>
#include <Elpida/Utilities/RawData.hpp>
#include "Benchmarks/Memory/WorkingSetSizes.hpp"

#include <cmath>

namespace Elpida
{

	MemoryRead::MemoryRead(const TaskSpecification& specification, const ProcessorNode& processorToRun)
		: Task(specification, processorToRun), _taskData(nullptr), _iterations(1)
	{

	}

	void MemoryRead::prepareImpl()
	{
		_taskData = getInput().getTaskData();
		if (_taskData->getSize() % (32 * sizeof(RegisterSize)) != 0)
			throw ElpidaException("Memory size must be divisible by the size of each read * 32!");

		_iterations = (unsigned long)std::max((_iterationConstant / (double)_taskData->getSize()), 1.0);
	}

	TaskDataDto MemoryRead::finalizeAndGetOutputData()
	{
		return TaskDataDto(*_taskData);
	}

	double MemoryRead::calculateTaskResultValue(const Duration& taskElapsedTime) const
	{
		return (double)_taskData->getSize() * (double)_iterations;
	}

	void MemoryRead::execute()
	{
		volatile auto* ptr = (RegisterSize*)_taskData->getData();
		const auto iterations = _iterations;
		const auto size = _taskData->getSize() / sizeof(RegisterSize);
		for (auto i = 0ul; i < iterations; ++i)
		{
			for (auto j = 0ul; j < size; j += 32)
			{
				ptr[j];
				ptr[j + 1];
				ptr[j + 2];
				ptr[j + 3];
				ptr[j + 4];
				ptr[j + 5];
				ptr[j + 6];
				ptr[j + 7];
				ptr[j + 8];
				ptr[j + 9];
				ptr[j + 10];
				ptr[j + 11];
				ptr[j + 12];
				ptr[j + 13];
				ptr[j + 14];
				ptr[j + 15];
				ptr[j + 16];
				ptr[j + 17];
				ptr[j + 18];
				ptr[j + 19];
				ptr[j + 20];
				ptr[j + 21];
				ptr[j + 22];
				ptr[j + 23];
				ptr[j + 24];
				ptr[j + 25];
				ptr[j + 26];
				ptr[j + 27];
				ptr[j + 28];
				ptr[j + 29];
				ptr[j + 30];
				ptr[j + 31];
			}
		}
	}

} /* namespace Elpida */

