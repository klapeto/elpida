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

#include "Elpida/Utilities/MemoryFile.hpp"
#include "Elpida/Engine/Task/TaskDataDto.hpp"
#include "Elpida/Utilities/RawData.hpp"

namespace Elpida
{

	WriteFile::WriteFile(const TaskSpecification& specification,
		const ProcessorNode& processorToRun,
		std::string filePath,
		size_t iterationsToRun)
		: Task(specification, processorToRun, iterationsToRun),
		  _outputPath(std::move(filePath))
	{

	}

	void WriteFile::execute()
	{
		const auto& input = getInput();
		MemoryFile(input.getTaskData()->getData(), input.getTaskData()->getSize()).writeToFile(_outputPath);
	}

	void WriteFile::prepareImpl()
	{

	}

	TaskDataDto WriteFile::finalizeAndGetOutputData()
	{
		return TaskDataDto();
	}

	double WriteFile::calculateTaskResultValue(const Duration& taskElapsedTime) const
	{
		return getInput().getTaskData() ? getInput().getTaskData()->getSize() : 0.0;
	}
} /* namespace Elpida */

