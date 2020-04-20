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
#include "Elpida/Engine/Task/TaskData.hpp"

namespace Elpida
{

	ReadFile::ReadFile(const TaskSpecification& specification,
		const TaskAffinity& affinity,
		std::string filePath,
		bool toBeCountedOnResults)
		: Task(specification, affinity, toBeCountedOnResults), _filePath(std::move(filePath))
	{

	}

	ReadFile::~ReadFile()
	{
		delete _outputData;
	}

	void ReadFile::run()
	{
		_file.load(_filePath);
	}

	void ReadFile::finalize()
	{
		_outputData = new TaskData(static_cast<unsigned char*>(_file.getData()), _file.getSize());
	}

	void ReadFile::prepare()
	{

	}

} /* namespace Elpida */
