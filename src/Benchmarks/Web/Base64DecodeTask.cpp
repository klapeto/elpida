//
//  Copyright (c) 2024  Ioannis Panagiotopoulos
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "Base64DecodeTask.hpp"

#include "Base64.hpp"

#include <cstring>

namespace Elpida
{

	void Base64DecodeTask::Prepare(SharedPtr<AbstractTaskData> inputData)
	{
		_inputData = std::move(inputData);
	}

	SharedPtr<AbstractTaskData> Base64DecodeTask::Finalize()
	{
		_inputData->ReAllocate(_outputData.size());
		std::memcpy(_inputData->GetData(), _outputData.data(), _outputData.size());
		return std::move(_inputData);
	}

	Size Base64DecodeTask::GetProcessedDataSize() const
	{
		return _inputData->GetSize();
	}

	void Base64DecodeTask::DoRun(Iterations iterations)
	{
		auto data = _inputData->GetData();
		auto size = _inputData->GetSize();
		while (iterations-- > 0)
		{
			_outputData = Base64::Decode(data, size);
		}
	}

	Size Base64DecodeTask::GetOperationsPerformedPerRun()
	{
		return 1;
	}

	UniquePtr<Task> Base64DecodeTask::DoDuplicate() const
	{
		return std::make_unique<Base64DecodeTask>();
	}

	TaskInfo Base64DecodeTask::DoGetInfo() const
	{
		return { "Base64 Decode",
				 "Decodes base64 encoded data",
				 "B",
				 "The data process rate",
				 ResultType::Throughput
		};
	}

}