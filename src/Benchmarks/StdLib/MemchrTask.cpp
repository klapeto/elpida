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

#include "MemchrTask.hpp"

#include <cstring>

namespace Elpida
{

	const char Character = 'o';

	void MemchrTask::Prepare(SharedPtr<AbstractTaskData> inputData)
	{
		_inputData = std::move(inputData);
		std::memset(_inputData->GetData(), 0, _inputData->GetSize());
		_inputData->GetData()[_inputData->GetSize() - 1] = Character;
	}

	SharedPtr<AbstractTaskData> MemchrTask::Finalize()
	{
		return std::move(_inputData);
	}

	Size MemchrTask::GetProcessedDataSize() const
	{
		return _inputData->GetSize();
	}

	void MemchrTask::DoRunImpl()
	{
		auto data = _inputData->GetData();
		auto size = _inputData->GetSize();
		Exec([&]()
		{
			volatile auto x = std::memchr(data, Character, size);
		});
	}

	Size MemchrTask::GetOperationsPerformedPerRun()
	{
		return 1;
	}

	Duration MemchrTask::GetExecutionMinimumDuration()
	{
		return Elpida::Seconds(5);
	}

	UniquePtr<Task> MemchrTask::DoDuplicate() const
	{
		return std::make_unique<MemchrTask>();
	}

	TaskInfo MemchrTask::DoGetInfo() const
	{
		return { "Memchr",
				 "Memchr Description",
				 "B",
				 "The process rate",
				 ResultType::Throughput
		};
	}

}