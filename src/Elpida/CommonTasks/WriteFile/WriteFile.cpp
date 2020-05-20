/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2018  Ioannis Panagiotopoulos
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
#include "Elpida/ElpidaException.hpp"
#include "Elpida/Engine/Task/TaskInput.hpp"


namespace Elpida
{

	WriteFile::WriteFile(const TaskSpecification& specification,
		const TaskAffinity& affinity,
		std::string filePath)
		: Task(specification, affinity), _outputPath(std::move(filePath))
	{

	}

	void WriteFile::execute()
	{
		auto input = getInput().getTaskData().front();
		MemoryFile(input->getData(), input->getSize()).writeToFile(_outputPath);
	}

	void WriteFile::prepareImpl()
	{

	}

	TaskOutput WriteFile::finalizeAndGetOutputData()
	{
		return TaskOutput();
	}

	double WriteFile::calculateTaskResultValue(const Duration& taskElapsedTime) const
	{
		return getInput().getTaskData().front()->getSize();
	}
} /* namespace Elpida */

