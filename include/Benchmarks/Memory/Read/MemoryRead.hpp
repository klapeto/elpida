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
 * MemoryReadCached.hpp
 *
 *  Created on: 18 Οκτ 2018
 *      Author: klapeto
 */

#ifndef TASKBATCHES_MEMORY_READ_MEMORYREAD_HPP_
#define TASKBATCHES_MEMORY_READ_MEMORYREAD_HPP_

#include <Elpida/Engine/Task/Task.hpp>

namespace Elpida
{
	class TaskMetrics;

	class MemoryRead final : public Task
	{
	public:
		void execute() override;

		MemoryRead(const TaskSpecification& specification, const ProcessorNode& processorToRun, size_t iterationsToRun);
		~MemoryRead() override = default;
	protected:
		RawData* _taskData;
		unsigned long _iterations;
		static constexpr inline double _iterationConstant = 100000000000; // rough estimate

		void prepareImpl() override;
		TaskDataDto finalizeAndGetOutputData() override;
		[[nodiscard]] double calculateTaskResultValue(const Duration& taskElapsedTime) const override;
	};

} /* namespace Elpida */

#endif /* TASKBATCHES_MEMORY_READ_MEMORYREAD_HPP_ */
