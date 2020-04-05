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

#include "Elpida/CommonTasks/WriteFile.hpp"

#include "Elpida/TaskMetrics.hpp"
#include "Elpida/Utilities/MemoryFile.hpp"

namespace Elpida
{
	WriteFile::WriteFile(const DataPtr& data, const std::size_t& size, const std::string& outputPath)
		:
		Task("Write File: " + outputPath, false),
		_outputPath(outputPath),
		_runResult("Write rate", "bytes"),
		_data(data),
		_size(size)
	{

	}

	WriteFile::~WriteFile()
	{

	}

	void WriteFile::run()
	{
		MemoryFile(_data, _size).writeToFile(_outputPath);
	}

	void WriteFile::calculateResults(const TaskMetrics& metrics)
	{
		_runResult.setOriginalValue(_size);
		addResult(_runResult);
	}

} /* namespace Elpida */

