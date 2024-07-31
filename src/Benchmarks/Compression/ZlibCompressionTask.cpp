//
// Created by klapeto on 30/7/2024.
//

#include "ZlibCompressionTask.hpp"
#include "Elpida/Core/ElpidaException.hpp"
#include "ZlibCompressor.hpp"

#include <memory>

namespace Elpida
{
	void ZlibCompressionTask::Prepare(SharedPtr<AbstractTaskData> inputData)
	{
		_output.reset();
		_input = std::move(inputData);
		_output = MemoryStream(_input->GetAllocator());
		ZlibCompressor::Compress(_input->GetData(), _input->GetSize(), _output.value());
	}

	SharedPtr<AbstractTaskData> ZlibCompressionTask::Finalize()
	{
		return std::make_shared<RawTaskData>(_output.value().Detach());
	}

	Size ZlibCompressionTask::GetProcessedDataSize() const
	{
		return _input->GetSize();
	}

	void ZlibCompressionTask::DoRun(Iterations iterations)
	{
		while (iterations-- > 0)
		{
			_output->Reset();
			ZlibCompressor::Compress(_input->GetData(), _input->GetSize(), _output.value());
		}
	}

	TaskInfo ZlibCompressionTask::DoGetInfo() const
	{
		return TaskInfo("Zlib compression",
				"Compresses data with Zlib",
				"B",
				"The rate of compression",
				ResultType::Throughput);
	}

	Size ZlibCompressionTask::GetOperationsPerformedPerRun()
	{
		return 1;
	}

	Duration ZlibCompressionTask::GetExecutionMinimumDuration()
	{
		return Seconds(5);
	}

	UniquePtr<Task> ZlibCompressionTask::DoDuplicate() const
	{
		return std::make_unique<ZlibCompressionTask>();
	}
} // Elpida