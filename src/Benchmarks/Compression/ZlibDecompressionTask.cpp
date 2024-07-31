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
		ZlibCompressor::Decompress(_input->GetData(), _input->GetSize(), _output.value());
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

	Duration ZlibDecompressionTask::GetExecutionMinimumDuration()
	{
		return Seconds(5);
	}

	UniquePtr<Task> ZlibDecompressionTask::DoDuplicate() const
	{
		return std::make_unique<ZlibDecompressionTask>();
	}

} // Elpida