//
// Created by klapeto on 2/3/2023.
//

#include "FileReadTask.hpp"

#include <filesystem>
#include <fstream>

namespace Elpida
{
	void FileReadTask::Prepare(TaskData&& inputData)
	{
		if (!std::filesystem::exists(_filePath)){
			//throw;
		}

		_outputData.Allocate(std::filesystem::file_size(_filePath));

		_fileStream = std::fstream(std::filesystem::u8path(_filePath), std::ios::in | std::ios::binary);
	}

	TaskData FileReadTask::Finalize()
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
			ScoreType::Throughput,
			Elpida::DataInfo(),
			Elpida::DataInfo()
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
			_fileStream.read((char*)_outputData.GetDataRaw(), _outputData.GetSize());
		}
		catch (const std::fstream::failure& e)
		{
			//throw ElpidaException(FUNCTION_NAME,Vu::Cs("Failed to read file: '", _filePath, "'. Error: ", e.what()));
		}
	}

	std::unique_ptr<Task> FileReadTask::DoDuplicate() const
	{
		return std::unique_ptr<Task>(new FileReadTask(_filePath));
	}
} // Elpida