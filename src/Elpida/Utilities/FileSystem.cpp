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

#ifdef ELPIDA_LINUX
#include <dirent.h>
#include <sys/stat.h>
#include <cstring>
#else
#include <Windows.h>
#include <strsafe.h>
#include "Elpida/Utilities/WindowsUtils.hpp"
#endif

namespace Elpida
{
	void FileSystem::iterateDirectoryImpl(const char* path, std::function<void(const std::string&)>& func)
	{
#ifdef ELPIDA_LINUX
		DIR* dir;
		if ((dir = opendir(path)) != nullptr)
		{
			try
			{
				dirent* dirent;
				while ((dirent = readdir(dir)) != nullptr)
				{
					if (dirent->d_type == DT_DIR)
					{
						if (strcmp(dirent->d_name, ".") == 0 || strcmp(dirent->d_name, "..") == 0) continue;
						iterateDirectoryImpl(concatPaths(path, static_cast<const char*>(dirent->d_name)).c_str(), func);
					}
					else
					{
						func(concatPaths(path, static_cast<const char*>(dirent->d_name)));
					}
				}
			}
			catch (...)
			{
				closedir(dir);
				throw;
			}
			closedir(dir);
		}
		else
		{
			throw ElpidaException("iterateDirectory", "'" + std::string(path) + "' directory could not be opened.");
		}
#else
		WIN32_FIND_DATA data;
		HANDLE hFind = FindFirstFile(concatPaths(path, "*").c_str(), &data);

		if (hFind != INVALID_HANDLE_VALUE)
		{
			try
			{
				do
				{
					if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					{
						if (strcmp(data.cFileName, ".") == 0 || strcmp(data.cFileName, "..") == 0) continue;
						iterateDirectoryImpl(concatPaths(path, static_cast<const char*>(data.cFileName)).c_str(), func);
						continue;
					}
					func(concatPaths(path, data.cFileName));
				} while (FindNextFile(hFind, &data));
			}
			catch (...)
			{
				FindClose(hFind);
				throw;
			}
			FindClose(hFind);
		}
		else
		{
			throw ElpidaException("Iterate directory", WindowsUtils::GetLastErrorString());
		}

#endif
	}

	void FileSystem::iterateDirectory(const std::string& directory, std::function<void(const std::string&)> func)
	{
		iterateDirectoryImpl(directory.c_str(), func);
	}

	bool FileSystem::fileExists(const std::string& file)
	{
#ifdef ELPIDA_LINUX
		struct stat dummy;
		return stat(file.c_str(), &dummy) == 0;
#else
		auto attrib = GetFileAttributes(file.c_str());
		return attrib != INVALID_FILE_ATTRIBUTES;
#endif
	}
} /* namespace Elpida */
