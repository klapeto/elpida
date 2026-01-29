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

#include "StrtoulTask.hpp"

namespace Elpida
{

	void StrtoulTask::Prepare(SharedPtr<AbstractTaskData> inputData)
	{
		_inputData = std::move(inputData);
	}

	SharedPtr<AbstractTaskData> StrtoulTask::Finalize()
	{
		return std::move(_inputData);
	}

	Size StrtoulTask::GetProcessedDataSize() const
	{
		return sizeof("1524515736");
	}

	void StrtoulTask::DoRunImpl()
	{
		Exec([&]()
		{
			auto value = std::strtoul("1524515736", nullptr, 10);
		});
	}

	Size StrtoulTask::GetOperationsPerformedPerRun()
	{
		return 1;
	}

	Duration StrtoulTask::GetExecutionMinimumDuration()
	{
		return Elpida::Seconds(5);
	}

	UniquePtr<Task> StrtoulTask::DoDuplicate() const
	{
		return std::make_unique<StrtoulTask>();
	}

	TaskInfo StrtoulTask::DoGetInfo() const
	{
		return { "Strtoul",
				 "Measures the strtoul() performance",
				 "chars",
				 "The character process rate",
				 ResultType::Throughput
		};
	}

}