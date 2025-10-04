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

#include "SqrtTask.hpp"

#include <cmath>
#include "Elpida/Core/Repeat.hpp"
#include "Elpida/Core/Float.hpp"

namespace Elpida
{
	void SqrtTask::Prepare(SharedPtr<AbstractTaskData> inputData)
	{
		_inputData = std::move(inputData);
	}

	SharedPtr<AbstractTaskData> SqrtTask::Finalize()
	{
		return std::move(_inputData);
	}

	Size SqrtTask::GetProcessedDataSize() const
	{
		return 1;
	}

	void SqrtTask::DoRunImpl()
	{
		Float x = 156113132146186435.1513123;
		Exec([&]()
		{
			REPEAT_10000(x = std::sqrt(x));
		});
	}

	Size SqrtTask::GetOperationsPerformedPerRun()
	{
		return 10000;
	}

	Duration SqrtTask::GetExecutionMinimumDuration()
	{
		return Elpida::Seconds(5);
	}

	UniquePtr<Task> SqrtTask::DoDuplicate() const
	{
		return std::make_unique<SqrtTask>();
	}

	TaskInfo SqrtTask::DoGetInfo() const
	{
		return { "sqrt()",
				 "Measures the sqrt() performance",
				 "numbers",
				 "The number process rate",
				 ResultType::Throughput
		};
	}

}