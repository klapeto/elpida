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

//
// Created by klapeto on 2/3/2023.
//

#include "FileWriteTask.hpp"

#include <filesystem>

namespace Elpida
{
	void FileWriteTask::Prepare(SharedPtr<AbstractTaskData> inputData)
	{
		_inputData = std::move(inputData);

		_fileStream = std::fstream(std::filesystem::u8path(_filePath), std::ios::out | std::ios::binary);
	}

	SharedPtr<AbstractTaskData> FileWriteTask::Finalize()
	{
		return std::move(_inputData);
	}

	TaskInfo FileWriteTask::DoGetInfo() const
	{
		return {
				"File write",
				"Writes a file from the memory to filesystem.",
				"B",
				"The amount of bytes written per second",
				ResultType::Throughput
		};
	}

	ConcurrencyMode FileWriteTask::GetAllowedConcurrency() const
	{
		return ConcurrencyMode::None;
	}

	FileWriteTask::FileWriteTask(std::string filePath)
			:_filePath(std::move(filePath))
	{

	}

	void FileWriteTask::DoRun()
	{
		_fileStream.write((char*)_inputData->GetData(), _inputData->GetSize());
		_fileStream.flush();
	}

	std::unique_ptr<Task> FileWriteTask::DoDuplicate() const
	{
		return std::unique_ptr<Task>(new FileWriteTask(_filePath));
	}

	Size FileWriteTask::GetProcessedDataSize() const
	{
		return _inputData->GetSize();
	}
} // Elpida