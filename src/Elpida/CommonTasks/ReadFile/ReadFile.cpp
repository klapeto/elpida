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
#include "Elpida/Topology/SystemTopology.hpp"
#include "Elpida/Topology/ProcessorNode.hpp"
#include "Elpida/Engine/Data/ActiveTaskData.hpp"

namespace Elpida
{

	ReadFile::ReadFile(const TaskSpecification& specification,
		const ProcessorNode& processorToRun,
		std::string filePath)
		: Task(specification, processorToRun),
		  _filePath(std::move(filePath)),
		  _data(nullptr)
	{

	}

	void ReadFile::execute()
	{
		std::ifstream file(_filePath, std::ifstream::binary);
		try
		{
			file.read((char*)_data->getData(), _data->getSize());
		}
		catch (...)
		{
			delete _data;
			if (file.is_open())
			{
				file.close();
			}
			throw;
		}
	}

	void ReadFile::prepareImpl()
	{
		std::ifstream file(_filePath, std::ifstream::binary);
		try
		{
			file.exceptions(std::ios::failbit);
			file.seekg(0, std::ifstream::end);
			size_t size = file.tellg();
			file.seekg(0, std::ifstream::beg);

			_data = new ActiveTaskData(size, SystemTopology::getNumaNodeOfProcessor((int)_processorToRun.getOsIndex()));
		}
		catch (...)
		{
			delete _data;
			if (file.is_open())
			{
				file.close();
			}
			throw;
		}
	}

	TaskDataDto ReadFile::finalizeAndGetOutputData()
	{
		return TaskDataDto(*_data);
	}

	double ReadFile::calculateTaskResultValue(const Duration& taskElapsedTime) const
	{
		return _data->getSize();
	}
} /* namespace Elpida */
