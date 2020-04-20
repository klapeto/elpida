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

#include "Elpida/Utilities/MemoryFile.hpp"
#include "Elpida/ElpidaException.hpp"
#include "Elpida/Engine/Task/TaskData.hpp"

namespace Elpida
{

	WriteFile::WriteFile(const TaskSpecification& specification,
		const TaskAffinity& affinity,
		std::string filePath,
		bool toBeCountedOnResults)
		: Task(specification, affinity, toBeCountedOnResults), _outputPath(filePath)
	{

	}

	void WriteFile::run()
	{
		MemoryFile((*_inputData)->getData(), (*_inputData)->getSize()).writeToFile(_outputPath);
	}

	void WriteFile::prepare()
	{
		if ((*_inputData) == nullptr)
		{
			throw ElpidaException("Write File", "Previous task did not provide input!");
		}
	}

	void WriteFile::finalize()
	{

	}
} /* namespace Elpida */

