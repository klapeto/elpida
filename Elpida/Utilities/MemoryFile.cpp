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
 * MemoryFile.cpp
 *
 *  Created on: 18 Μαρ 2018
 *      Author: klapeto
 */

#include "Elpida/Utilities/MemoryFile.hpp"

#include <fstream>

#include "Elpida/Exceptions/IOException.hpp"

namespace Elpida
{

	MemoryFile::MemoryFile()
			: _data(nullptr), _size(0), _deleteData(true)
	{

	}

	MemoryFile::MemoryFile(std::size_t size)
			: _size(size), _deleteData(true)
	{
		_data = new Data[_size];
	}

	MemoryFile::MemoryFile(void* data, std::size_t size)
			: _data((DataPtr) data), _size(size), _deleteData(false)
	{

	}

	MemoryFile::~MemoryFile()
	{
		if (_data != nullptr && _deleteData)
		{
			delete[] _data;
		}
	}

	void MemoryFile::load(const std::string& path)
	{
		if (_data != nullptr && _deleteData)
		{
			delete[] _data;
		}
		_size = 0;
		std::ifstream file(path, std::ifstream::binary);
		if (file.good())
		{
			file.seekg(0, file.end);
			_size = file.tellg();
			file.seekg(0, file.beg);

			_data = new Data[_size];
			file.read((char*) _data, _size);
			file.close();
		}
		else
		{
			throw IOException("MemoryFile", "Failed to open file: " + path);
		}
	}

	void MemoryFile::writeToFile(const std::string& path) const
	{
		std::ofstream file(path, std::ofstream::binary);
		if (file.good())
		{
			file.write((char*) _data, _size);
			file.flush();
			file.close();
		}
		else
		{
			throw IOException("MemoryFile", "Failed to write file: " + path);
		}
	}

} /* namespace Elpida */
