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

#include "CeilTask.hpp"

#include <cmath>
#include "Elpida/Core/Repeat.hpp"
#include "Elpida/Core/Float.hpp"

namespace Elpida
{

	void CeilTask::Prepare(SharedPtr<AbstractTaskData> inputData)
	{
		_inputData = std::move(inputData);
	}

	SharedPtr<AbstractTaskData> CeilTask::Finalize()
	{
		return std::move(_inputData);
	}

	Size CeilTask::GetProcessedDataSize() const
	{
		return 1;
	}

	void CeilTask::DoRunImpl()
	{
		Float y = 1561131321.1513123;
		Exec([&]()
		{
			REPEAT_10000(y = std::ceil(y));
		});
	}

	Size CeilTask::GetOperationsPerformedPerRun()
	{
		return 10000;
	}

	Duration CeilTask::GetExecutionMinimumDuration()
	{
		return Elpida::Seconds(5);
	}

	UniquePtr<Task> CeilTask::DoDuplicate() const
	{
		return std::make_unique<CeilTask>();
	}

	TaskInfo CeilTask::DoGetInfo() const
	{
		return { "ceil()",
				 "Measures the ceil() performance",
				 "number",
				 "The number process rate",
				 ResultType::Throughput
		};
	}

}