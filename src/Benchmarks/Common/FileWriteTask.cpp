//
// Created by klapeto on 2/3/2023.
//

#include "FileWriteTask.hpp"

#include <filesystem>

namespace Elpida
{
	void FileWriteTask::Prepare(UniquePtr<AbstractTaskData> inputData)
	{
		_inputData = std::move(inputData);

		_fileStream = std::fstream(std::filesystem::u8path(_filePath), std::ios::out | std::ios::binary);
	}

	UniquePtr<AbstractTaskData> FileWriteTask::Finalize()
	{
		return std::move(_inputData);
	}

	TaskInfo FileWriteTask::GetInfo() const
	{
		return {
			"File write",
			"Writes a file from the memory to filesystem.",
			"B",
			"The amount of bytes written per second",
			ScoreType::Throughput
		};
	}
	bool FileWriteTask::CanBeMultiThreaded() const
	{
		return false;
	}

	FileWriteTask::FileWriteTask(std::string filePath)
		: _filePath(std::move(filePath))
	{

	}

	void FileWriteTask::DoRun()
	{
		_fileStream.write((char*)_inputData->GetDataRaw(), _inputData->GetSize());
	}

	std::unique_ptr<Task> FileWriteTask::DoDuplicate() const
	{
		return std::unique_ptr<Task>(new FileWriteTask(_filePath));
	}
} // Elpida