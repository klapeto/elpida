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

#include <random>
#include "StrtodTask.hpp"

namespace Elpida
{
	void StrtodTask::Prepare(SharedPtr<AbstractTaskData> inputData)
	{
		_inputData = std::move(inputData);
	}

	SharedPtr<AbstractTaskData> StrtodTask::Finalize()
	{
		return std::move(_inputData);
	}

	Size StrtodTask::GetProcessedDataSize() const
	{
		return sizeof("53215.1513123");
	}

	void StrtodTask::DoRunImpl()
	{
		Exec([&]()
		{
			auto value = std::strtod("53215.1513123", nullptr);
		});
	}

	Size StrtodTask::GetOperationsPerformedPerRun()
	{
		return 1;
	}

	Duration StrtodTask::GetExecutionMinimumDuration()
	{
		return Elpida::Seconds(5);
	}

	UniquePtr<Task> StrtodTask::DoDuplicate() const
	{
		return std::make_unique<StrtodTask>();
	}

	TaskInfo StrtodTask::DoGetInfo() const
	{
		return { "Strtod",
				 "Measures the strtod() performance",
				 "chars",
				 "The character process rate",
				 ResultType::Throughput
		};
	}
}