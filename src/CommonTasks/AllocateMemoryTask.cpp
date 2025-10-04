/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

//
// Created by klapeto on 2/6/2024.
//

#include "AllocateMemoryTask.hpp"

namespace Elpida
{
	void AllocateMemoryTask::Prepare(SharedPtr<AbstractTaskData> inputData)
	{
		_outputData = std::make_unique<RawTaskData>(inputData->GetAllocator());
	}

	SharedPtr<AbstractTaskData> AllocateMemoryTask::Finalize()
	{
		return std::move(_outputData);
	}

	ConcurrencyMode AllocateMemoryTask::GetAllowedConcurrency() const
	{
		return ConcurrencyMode::None;
	}

	Size AllocateMemoryTask::GetProcessedDataSize() const
	{
		return _size;
	}

	AllocateMemoryTask::AllocateMemoryTask(std::size_t size)
			:_size(size)
	{

	}

	void AllocateMemoryTask::DoRun()
	{
		_outputData->Allocate(_size);
	}

	std::unique_ptr<Task> AllocateMemoryTask::DoDuplicate() const
	{
		return std::make_unique<AllocateMemoryTask>(_size);
	}

	TaskInfo AllocateMemoryTask::DoGetInfo() const
	{
		return {
				"Allocate memory",
				"Allocates a chunk of memory to be used by other tasks.",
				"B",
				"The amount of bytes allocated per second",
				ResultType::Throughput
		};
	}
} // Elpida