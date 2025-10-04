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

#include "MemsetTask.hpp"

#include <cstring>

namespace Elpida
{

	void MemsetTask::Prepare(SharedPtr<AbstractTaskData> inputData)
	{
		_inputData = std::move(inputData);
	}

	SharedPtr<AbstractTaskData> MemsetTask::Finalize()
	{
		return std::move(_inputData);
	}

	Size MemsetTask::GetProcessedDataSize() const
	{
		return _inputData->GetSize();
	}

	void MemsetTask::DoRunImpl()
	{
		volatile auto data = _inputData->GetData();
		auto size = _inputData->GetSize();
		Exec([&]()
		{
			std::memset(data, 0, size);
		});
	}

	Size MemsetTask::GetOperationsPerformedPerRun()
	{
		return 1;
	}

	Duration MemsetTask::GetExecutionMinimumDuration()
	{
		return Elpida::Seconds(5);
	}

	UniquePtr<Task> MemsetTask::DoDuplicate() const
	{
		return std::make_unique<MemsetTask>();
	}

	TaskInfo MemsetTask::DoGetInfo() const
	{
		return { "Memset",
				 "Memset Description",
				 "B",
				 "The process rate",
				 ResultType::Throughput
		};
	}

}