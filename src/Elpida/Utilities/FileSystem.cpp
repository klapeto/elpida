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
 * FileSystem.cpp
 *
 *  Created on: 27 Οκτ 2018
 *      Author: klapeto
 */

#include "Elpida/Utilities/FileSystem.hpp"
#include "Elpida/ElpidaException.hpp"
#include "Elpida/Config.hpp"

#include <filesystem>

namespace fs = std::filesystem;

namespace Elpida
{
	void FileSystem::iterateDirectoryImpl(const char* path, std::function<void(const std::string&)>& func)
	{
		for (auto& dir: fs::recursive_directory_iterator(path))
		{
			if (dir.is_directory()) continue;
			func(dir.path().string());
		}
	}

	void FileSystem::iterateDirectory(const std::string& directory, std::function<void(const std::string&)> func)
	{
		iterateDirectoryImpl(directory.c_str(), func);
	}

	bool FileSystem::fileExists(const std::string& file)
	{
		return fs::exists(std::filesystem::u8path(file));
	}

	std::fstream FileSystem::openFile(const std::string& path, std::ios::openmode mode)
	{
		return std::fstream(fs::u8path(path), mode);
	}
} /* namespace Elpida */
