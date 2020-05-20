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
#include "Benchmarks/Memory/WorkingSetSizes.hpp"

namespace Elpida
{

	class MemoryReadLatency : public Task
	{
	public:

		void execute() override;

//		void calculateResults(const TaskMetrics& metrics) override
//		{
//			auto size = _iterations * _memory.getSize();
//			auto time = metrics.getDurationSubdivision<TaskMetrics::NanoSecond>();
//			_runResult.setOriginalValue((double)time / (double)size);
//			_runResult.setCustom(true);
//			_runResult.setMultiplier(1000);
//			_runResult.setTestedDataValue(_memory.getSize());
//			_runResult.setValueName("ns");
//			addResult(_runResult);
//		}

		explicit MemoryReadLatency(const TaskSpecification& specification,
			TaskAffinity affinity);

		~MemoryReadLatency() override = default;
	protected:
		void prepareImpl() override;
		TaskOutput finalizeAndGetOutputData() override;
		[[nodiscard]] double calculateTaskResultValue(const Duration& taskElapsedTime) const override;
	private:
		TaskData* _taskData;
		unsigned long _iterations;
		static constexpr inline double _iterationConstant =
			100000000000; // rough estimate, to be passed on construction later once we find the latency
	};

} /* namespace Elpida */

#endif /* TASKBATCHES_MEMORY_LATENCY_MEMORYLATENCY_HPP_ */
