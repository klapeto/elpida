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
 * MemoryFile.cpp
 *
 *  Created on: 18 Μαρ 2018
 *      Author: klapeto
 */

#include "Elpida/Utilities/MemoryFile.hpp"

#include <fstream>
#include "Elpida/Config.hpp"
#include "Elpida/ElpidaException.hpp"
#include "Elpida/Utilities/NumaMemory.hpp"
#include "Elpida/Topology/SystemTopology.hpp"

namespace Elpida
{

	MemoryFile::MemoryFile(unsigned int processorAffinity)
		: _data(nullptr), _allocatedMemory(nullptr), _size(0), _processorAffinity(processorAffinity), _deleteData(true)
	{

	}

	MemoryFile::MemoryFile(void* data, std::size_t size)
		: _data((pData)data), _allocatedMemory(nullptr), _size(size), _processorAffinity(-1), _deleteData(false)
	{

	}

	MemoryFile::~MemoryFile()
	{
		destroyData();
	}

	void MemoryFile::load(const std::string& path)
	{
		if (_deleteData)
		{
			destroyData();
			_size = 0;
			std::ifstream file(path, std::ifstream::binary);
			try
			{
				file.exceptions(std::ios::failbit);
				file.seekg(0, std::ifstream::end);
				_size = file.tellg();
				file.seekg(0, std::ifstream::beg);

				allocateData();
				file.read((char*)_data, _size);
			}
			catch (...)
			{
				destroyData();
				if (file.is_open())
				{
					file.close();
				}
				throw;
			}
		}
		else
		{
			throw ElpidaException(FUNCTION_NAME, "Attempted to load a file when it was constructed with existing data");
		}

	}

	void MemoryFile::destroyData()
	{
		if (_deleteData)
		{
			delete _allocatedMemory;
			_allocatedMemory = nullptr;
		}
	}

	void MemoryFile::writeToFile(const std::string& path) const
	{
		std::ofstream file(path, std::ofstream::binary);
		try
		{
			file.exceptions(std::ios::failbit);
			file.write((char*)_data, _size);
			file.flush();
			file.close();
		}
		catch (...)
		{
			if (file.is_open())
			{
				file.close();
			}
			throw;
		}
	}

	void MemoryFile::allocateData()
	{
		_allocatedMemory = new NumaMemory(_size, SystemTopology::getNumaNodeOfProcessor(_processorAffinity));
		_allocatedMemory->allocate();
		_data = static_cast<pData>(_allocatedMemory->getPointer());
	}

} /* namespace Elpida */
