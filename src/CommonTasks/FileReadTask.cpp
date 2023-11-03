//
// Created by klapeto on 2/3/2023.
//

#include "FileReadTask.hpp"

#include "Elpida/Core/ElpidaException.hpp"

#include <filesystem>
#include <fstream>

namespace Elpida
{
	void FileReadTask::Prepare(UniquePtr<AbstractTaskData> inputData)
	{
		if (!std::filesystem::exists(_filePath))
		{
			throw ElpidaException("File: '", _filePath, "' does not exist.");
		}

		_outputData = std::move(inputData);

		auto path = std::filesystem::u8path(_filePath);
		auto size = std::filesystem::file_size(_filePath);
		_outputData->Allocate(size);

		_fileStream = std::fstream(path, std::ios::in | std::ios::binary);
	}

	UniquePtr<AbstractTaskData> FileReadTask::Finalize()
	{
		if (_fileStream.is_open())
		{
			_fileStream.close();
		}

		return std::move(_outputData);
	}

	TaskInfo FileReadTask::GetInfo() const
	{
		return {
			"File read",
			"Reads a file from the filesystem to memory.",
			"B",
			"The amount of bytes read per second",
			ScoreType::Throughput
		};
	}
	bool FileReadTask::CanBeMultiThreaded() const
	{
		return false;
	}

	FileReadTask::FileReadTask(std::string filePath)
		: _filePath(std::move(filePath))
	{

	}

	void FileReadTask::DoRun()
	{
		try
		{
			_fileStream.read((char*)_outputData->GetData(), _outputData->GetSize());
		}
		catch (const std::fstream::failure& e)
		{
			throw ElpidaException("Failed to read file: '", _filePath, "'. Error: ", e.what());
		}
	}

	std::unique_ptr<Task> FileReadTask::DoDuplicate() const
	{
		return std::unique_ptr<Task>(new FileReadTask(_filePath));
	}

	Size FileReadTask::GetProcessedDataSize() const
	{
		return _outputData->GetSize();
	}
} // Elpida