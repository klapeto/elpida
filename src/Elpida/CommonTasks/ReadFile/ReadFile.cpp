/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2020  Ioannis Panagiotopoulos
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>
 *************************************************************************/

/*
 * ReadFile.cpp
 *
 *  Created on: 18 Μαρ 2018
 *      Author: klapeto
 */

#include "Elpida/CommonTasks/ReadFile/ReadFile.hpp"

#include <utility>
#include <fstream>
#include <filesystem>

#include "Elpida/Config.hpp"
#include "Elpida/SystemInfo/SystemTopology.hpp"
#include "Elpida/SystemInfo/ProcessorNode.hpp"
#include "Elpida/Engine/Data/RawTaskData.hpp"
#include "Elpida/Utilities/ValueUtilities.hpp"
#include "Elpida/ElpidaException.hpp"

namespace Elpida
{

	ReadFile::ReadFile(const TaskSpecification& specification,
		const ProcessorNode& processorToRun,
		std::string filePath, size_t iterationsToRun)
		: Task(specification, processorToRun, iterationsToRun),
		  _filePath(std::move(filePath)),
		  _size(0)
	{

	}

	void ReadFile::prepareImpl()
	{
		_size = 0;
		auto path = std::filesystem::u8path(_filePath);

		if (!std::filesystem::exists(path))
		{
			throw ElpidaException(FUNCTION_NAME,
				Vu::Cs("Failed to read file: '", _filePath, "' does not exist."));
		}

		auto file = std::fstream(path, std::ios::in | std::ios::binary);
		try
		{
			file.exceptions(std::ios::badbit | std::ios::failbit);
			file.seekg(0, std::ifstream::end);
			_size = file.tellg();
			file.seekg(0, std::ifstream::beg);

			if (_size == 0)
			{
				throw ElpidaException(FUNCTION_NAME,
					Vu::Cs("Failed to read file: '", _filePath, "' has no data."));
			}

			_data = std::make_unique<RawTaskData>(_size, _processorToRun);
		}
		catch (const std::fstream::failure& e)
		{
			_data.reset();
			if (file.is_open())
			{
				file.close();
			}
			throw ElpidaException(FUNCTION_NAME,
				Vu::Cs("Failed to read file: '", _filePath, "'. Error: ", e.what()));
		}
	}

	void ReadFile::execute()
	{
		auto file = std::fstream(std::filesystem::u8path(_filePath), std::ios::in | std::ios::binary);
		try
		{
			file.read((char*)_data->getData(), _data->getSize());
		}
		catch (const std::fstream::failure& e)
		{
			_data.reset();
			if (file.is_open())
			{
				file.close();
			}
			throw ElpidaException(FUNCTION_NAME,
				Vu::Cs("Failed to read file: '", _filePath, "'. Error: ", e.what()));
		}
	}

	std::optional<TaskDataDto> ReadFile::finalizeAndGetOutputData()
	{
		return TaskDataDto(std::move(_data));
	}

	double ReadFile::calculateTaskResultValue(const Duration& taskElapsedTime) const
	{
		return _size;
	}
} /* namespace Elpida */
