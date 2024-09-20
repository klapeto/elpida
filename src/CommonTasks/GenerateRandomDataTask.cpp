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

//
// Created by klapeto on 20/9/2024.
//

#include "GenerateRandomDataTask.hpp"

#include "Elpida/Core/RawTaskData.hpp"

#include <random>

namespace Elpida
{
	void GenerateRandomDataTask::Prepare(SharedPtr<AbstractTaskData> inputData)
	{
		_outputData = std::make_unique<RawTaskData>(inputData->GetAllocator());

		_outputData->Allocate(_size);
	}

	SharedPtr<AbstractTaskData> GenerateRandomDataTask::Finalize()
	{
		return std::move(_outputData);
	}

	ConcurrencyMode GenerateRandomDataTask::GetAllowedConcurrency() const
	{
		return ConcurrencyMode::None;
	}

	Size GenerateRandomDataTask::GetProcessedDataSize() const
	{
		return _size;
	}

	GenerateRandomDataTask::GenerateRandomDataTask(std::size_t size)
			:_size(size)
	{

	}

	void GenerateRandomDataTask::DoRun()
	{
		auto data = _outputData->GetData();
		auto generator = std::mt19937(12345); // NOLINT(*-msc51-cpp)
		for (std::size_t i = 0; i < _size; ++i)
		{
			data[i] = generator() / (std::mt19937::max() / 256);
		}
	}

	std::unique_ptr<Task> GenerateRandomDataTask::DoDuplicate() const
	{
		return std::make_unique<GenerateRandomDataTask>(_size);
	}

	TaskInfo GenerateRandomDataTask::DoGetInfo() const
	{
		return {
				"Generate random data",
				"Generates random data",
				"b",
				"The rate of the generation",
				ResultType::Throughput,
				false
		};
	}
} // Elpida