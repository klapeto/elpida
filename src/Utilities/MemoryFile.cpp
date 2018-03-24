/*
 * MemoryFile.cpp
 *
 *  Created on: 18 Μαρ 2018
 *      Author: klapeto
 */

#include "Utilities/MemoryFile.hpp"
#include <fstream>

namespace Elpida
{

	MemoryFile::MemoryFile() :
			_data(nullptr),
			_size(0),
			_deleteData(true)
	{

	}

	MemoryFile::MemoryFile(size_t size) :
			_size(size),
			_deleteData(true)
	{
		_data = new unsigned char[_size];
	}

	MemoryFile::MemoryFile(void* data, size_t size) :
			_data((unsigned char*) data),
			_size(size),
			_deleteData(false)
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

			_data = new unsigned char[_size];
			file.read((char*) _data, _size);
			file.close();
		}
	}

	void MemoryFile::writeToFile(const std::string& path) const
	{
		std::ofstream file(path, std::ofstream::binary);
		if (file.good())
		{
			file.write((char*)_data, _size);
			file.flush();
			file.close();
		}
	}

} /* namespace Elpida */
