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
 * MemoryLatency.hpp
 *
 *  Created on: 19 Μαΐ 2019
 *      Author: klapeto
 */

#ifndef TASKBATCHES_MEMORY_LATENCY_MEMORYLATENCY_HPP_
#define TASKBATCHES_MEMORY_LATENCY_MEMORYLATENCY_HPP_

#include <cstdint>
#include <string>
#include <random>

#include <Elpida/Engine/Task/Task.hpp>

namespace Elpida
{

	class MemoryReadLatency : public Task
	{
	public:

		void execute() override;

		explicit MemoryReadLatency(const TaskSpecification& specification,
			const ProcessorNode& processorToRun,
			size_t iterationsToRun);

		~MemoryReadLatency() override = default;
	protected:
		void prepareImpl() override;
		[[nodiscard]] double calculateTaskResultValue(const Duration& taskElapsedTime) const override;
	private:
		const RawTaskData* _taskData;
		unsigned long _iterations;
		static constexpr inline double _iterationConstant =
			1000000000; // rough estimate, to be passed on construction later once we find the latency
	};

} /* namespace Elpida */

#endif /* TASKBATCHES_MEMORY_LATENCY_MEMORYLATENCY_HPP_ */
