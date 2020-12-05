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

//
// Created by klapeto on 30/4/20.
//

#include "Benchmarks/Memory/Latency/MemoryReadLatencySpecification.hpp"

#include "Benchmarks/Memory/Latency/MemoryReadLatency.hpp"
#include "Benchmarks/Memory/WorkingSetSizes.hpp"

namespace Elpida
{

	MemoryReadLatencySpecification::MemoryReadLatencySpecification()
		: TaskSpecification("Memory Read Latency",
		ResultSpecification("Access time", "s", ResultType::Raw, ResultSpecification::Average))
	{
		withDescription("Reads continuously a memory region to determine memory read latency");
		withInputData(DataSpecification("Input Memory",
			"B",
			32 * sizeof(RegisterSize),
			"The allocated memory region to read"));
	}

	Task* MemoryReadLatencySpecification::createNewTask(const TaskConfiguration& configuration,
		const ProcessorNode& processorToRun,
		size_t iterationsToRun) const
	{
		return new MemoryReadLatency(*this, processorToRun, iterationsToRun);
	}
}