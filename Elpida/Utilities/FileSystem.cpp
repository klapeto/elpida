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
#elif _elpida_windows
#include <Windows.h>
#include <strsafe.h>
#endif

namespace Elpida
{
	void FileSystem::iterateDirectory(const String& directory, std::function<void(const String&)> func)
	{
#if _elpida_linux
		DIR *dir;
		dirent *dirent;
		if ((dir = opendir(directory.c_str())) != nullptr)
		{
			while ((dirent = readdir(dir)) != nullptr)
			{
				func(directory + PATH_SEPERATOR + dirent->d_name);
			}
			closedir(dir);
		}
		else
		{
			throw IOException("iterateDirectory", "'" + directory + "' directory could not be opened.");
		}

#elif _elpida_windows
		WIN32_FIND_DATA data;
		HANDLE hFind = FindFirstFile((directory + PATH_SEPERATOR + "*").c_str(), &data);

		if (hFind != INVALID_HANDLE_VALUE)
		{
			do
			{
				func(directory + PATH_SEPERATOR + data.cFileName);
			}while (FindNextFile(hFind, &data));
			FindClose(hFind);
		}

#endif
	}

	bool FileSystem::fileExists(const String& file)
	{
		std::ifstream fp(file);
		if (fp.good())
		{
			fp.close();
			return true;
		}
		return false;
	}
} /* namespace Elpida */
