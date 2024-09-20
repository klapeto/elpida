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
// Created by klapeto on 31/7/2024.
//

#include "ZlibDecompressionTask.hpp"
#include "ZlibCompressor.hpp"

namespace Elpida
{
	void ZlibDecompressionTask::Prepare(SharedPtr<AbstractTaskData> inputData)
	{
		_output.reset();
		_input = std::move(inputData);
		_output = MemoryStream(_input->GetAllocator());
		//ZlibCompressor::Decompress(_input->GetData(), _input->GetSize(), _output.value());
	}

	SharedPtr<AbstractTaskData> ZlibDecompressionTask::Finalize()
	{
		return std::make_shared<RawTaskData>(_output->Detach());
	}

	Size ZlibDecompressionTask::GetProcessedDataSize() const
	{
		return _input->GetSize();
	}

	void ZlibDecompressionTask::DoRun(Iterations iterations)
	{
		while (iterations-- > 0)
		{
			_output->Reset();
			ZlibCompressor::Decompress(_input->GetData(), _input->GetSize(), _output.value());
		}
	}

	TaskInfo ZlibDecompressionTask::DoGetInfo() const
	{
		return TaskInfo("Zlib Decompression",
				"Decompresses data with Zlib",
				"B",
				"The rate of decompression",
				ResultType::Throughput);
	}

	Size ZlibDecompressionTask::GetOperationsPerformedPerRun()
	{
		return 1;
	}

	UniquePtr<Task> ZlibDecompressionTask::DoDuplicate() const
	{
		return std::make_unique<ZlibDecompressionTask>();
	}

} // Elpida