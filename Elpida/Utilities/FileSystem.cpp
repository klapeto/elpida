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
 * FileSystem.cpp
 *
 *  Created on: 27 Οκτ 2018
 *      Author: klapeto
 */

#include "Elpida/Utilities/FileSystem.hpp"
#include "Elpida/Exceptions/IOException.hpp"

#include <fstream>

#if _elpida_linux
#include <dirent.h>
#include <sys/stat.h>
#elif _elpida_windows
#include <Windows.h>
#include <strsafe.h>
#endif

namespace Elpida
{
	void FileSystem::iterateDirectory(const std::string& directory, std::function<void(const std::string&)> func)
	{
#if _elpida_linux
		DIR *dir;
		dirent *dirent;
		if ((dir = opendir(directory.c_str())) != nullptr)
		{
			while ((dirent = readdir(dir)) != nullptr)
			{
				func(concatPaths(directory, dirent->d_name));
			}
			closedir(dir);
		}
		else
		{
			throw IOException("iterateDirectory", "'" + directory + "' directory could not be opened.");
		}

#elif _elpida_windows
		WIN32_FIND_DATA data;
		HANDLE hFind = FindFirstFile(concatPaths(directory, "*").c_str(), &data);

				if (hFind != INVALID_HANDLE_VALUE)
				{
					do
					{
						func(concatPaths(directory, data.cFileName));
					}while (FindNextFile(hFind, &data));
					FindClose(hFind);
				}

#endif
	}

	bool FileSystem::fileExists(const std::string& file)
	{
#if _elpida_linux
		struct stat dummy;
		return stat(file.c_str(), &dummy) == 0;
#elif _elpida_windows
		auto attrib = GetFileAttributes(file.c_str());
		return attrib != INVALID_FILE_ATTRIBUTES;
#endif
	}
} /* namespace Elpida */
