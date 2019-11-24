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

#include "TaskBatches/General/ReadFile.hpp"

#include "Elpida/TaskMetrics.hpp"

namespace Elpida
{

	ReadFile::ReadFile(const std::string& filePath)
			: Task("Read File: " + filePath, false), _runResult("Read rate", "Bytes"), _filePath(filePath)
	{
	}

	ReadFile::~ReadFile()
	{

	}

	void ReadFile::calculateResults(const TaskMetrics& metrics)
	{
		_runResult.setOriginalValue(_file.getSize());
		addResult(_runResult);
	}

	void ReadFile::run()
	{
		_file.load(_filePath);
	}

} /* namespace Elpida */
