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
 * MemoryLatencyTaskBatch.hpp
 *
 *  Created on: 19 Μαΐ 2019
 *      Author: klapeto
 */

#ifndef TASKBATCHES_MEMORY_LATENCY_MEMORYLATENCYTASKBATCH_HPP_
#define TASKBATCHES_MEMORY_LATENCY_MEMORYLATENCYTASKBATCH_HPP_

#include <cstddef>

#include "Elpida/TaskBatch.hpp"

namespace Elpida
{

	class MemoryLatencyTaskBatch final : public TaskBatch
	{
	public:

		void createTasks() const override;

		MemoryLatencyTaskBatch()
			: TaskBatch("Memory Latency")
		{

		}
		~MemoryLatencyTaskBatch()
		{
		}
	private:
		void addMemoryLatencyTask(std::size_t size) const;
	};

} /* namespace Elpida */

#endif /* TASKBATCHES_MEMORY_LATENCY_MEMORYLATENCYTASKBATCH_HPP_ */
