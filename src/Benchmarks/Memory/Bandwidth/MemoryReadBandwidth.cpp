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

#include "Benchmarks/Memory/Bandwidth/MemoryReadBandwidth.hpp"

#include <Elpida/Utilities/Duration.hpp>
#include <Elpida/SystemInfo/MemoryInfo.hpp>
#include <Elpida/SystemInfo/CpuInfo.hpp>
#include "Elpida/Engine/ServiceProvider.hpp"
#include <Elpida/SystemInfo/TimingInfo.hpp>

#include "Benchmarks/Memory/WorkingSetSizes.hpp"
#include "Benchmarks/Memory/Bandwidth/MemoryReadBandwidthSpecification.hpp"

namespace Elpida
{

	MemoryReadBandwidth::MemoryReadBandwidth(const TaskSpecification& specification,
		const ProcessorNode& processorToRun,
		const ServiceProvider& serviceProvider,
		size_t memorySize,
		size_t iterationsToRun)
		: MicroTask(specification, processorToRun, serviceProvider, iterationsToRun),
		  _memory(memorySize, processorToRun),
		  _serviceProvider(serviceProvider)
	{

	}

	void MemoryReadBandwidth::prepareImpl()
	{
		_memory.allocate();
	}

	double MemoryReadBandwidth::getInputDataSize() const
	{
		return _memory.getSize();
	}

	double MemoryReadBandwidth::calculateTaskResultValue(const Duration& taskElapsedTime) const
	{
		return (double)_memory.getSize();
	}

	void MemoryReadBandwidth::run(size_t iterations)
	{
		volatile auto* ptr = (int*)_memory.getPointer();
		const auto size = _memory.getSize();

		while (iterations-- > 0)
		{
			for (auto j = 0ul; j < size / sizeof(int); j += 32)
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

	Duration MemoryReadBandwidth::getMinimumExecutionTime() const
	{
		return Seconds(2);
	}

} /* namespace Elpida */

