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
 * ReadFile.cpp
 *
 *  Created on: 18 Μαρ 2018
 *      Author: klapeto
 */

#include "Elpida/CommonTasks/ReadFile/ReadFile.hpp"

#include <utility>
#include "Elpida/Topology/ProcessorNode.hpp"
#include "Elpida/Engine/Task/TaskAffinity.hpp"
#include "Elpida/Engine/Data/PassiveTaskData.hpp"

namespace Elpida
{

	ReadFile::ReadFile(const TaskSpecification& specification,
		const TaskAffinity& affinity,
		std::string filePath)
		: Task(specification, affinity),
		  _file(affinity.getProcessorNodes().front()->getOsIndex()),
		  _filePath(std::move(filePath))
	{

	}

	void ReadFile::execute()
	{
		_file.load(_filePath);
	}

	void ReadFile::prepareImpl()
	{

	}

	TaskOutput ReadFile::finalizeAndGetOutputData()
	{
		return TaskOutput(new PassiveTaskData(_file.getData(), _file.getSize()));;
	}

	size_t ReadFile::calculateTaskResultValue(const Duration& taskElapsedTime) const
	{
		return _file.getSize();;
	}
} /* namespace Elpida */
