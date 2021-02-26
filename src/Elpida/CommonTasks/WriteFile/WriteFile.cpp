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
 * WriteFile.cpp
 *
 *  Created on: 18 Μαρ 2018
 *      Author: klapeto
 */

#include "Elpida/CommonTasks/WriteFile/WriteFile.hpp"

#include <utility>
#include <fstream>
#include <filesystem>

#include "Elpida/Config.hpp"
#include "Elpida/Utilities/ValueUtilities.hpp"
#include "Elpida/ElpidaException.hpp"
#include "Elpida/Engine/Data/RawTaskData.hpp"

namespace Elpida
{

	WriteFile::WriteFile(const TaskSpecification& specification,
		const ProcessorNode& processorToRun,
		const ServiceProvider& serviceProvider,
		std::string filePath,
		size_t iterationsToRun)
		: WorkloadTask(specification, processorToRun, serviceProvider, iterationsToRun),
		  _outputPath(std::move(filePath))
	{

	}

	void WriteFile::run()
	{
		auto& input = getInput();

		auto file = std::fstream(std::filesystem::u8path(_outputPath), std::ios::out | std::ios::binary);
		try
		{
			file.exceptions(std::ios::failbit);
			file.write((char*)input.getTaskData()->getData(), input.getTaskData()->getSize());
			file.flush();
			file.close();
		}
		catch (const std::fstream::failure& e)
		{
			if (file.is_open())
			{
				file.close();
			}
			throw ElpidaException(FUNCTION_NAME,
				Vu::Cs("Failed to read file: '", _outputPath, "'. Error: ", e.what()));
		}
	}

	double WriteFile::calculateTaskResultValue(const Duration& taskElapsedTime) const
	{
		return getInput().getTaskData()->getSize();
	}
} /* namespace Elpida */

