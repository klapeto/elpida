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
 * PluginLoader.cpp
 *
 *  Created on: 2 Ιουλ 2018
 *      Author: klapeto
 */

#include <iostream>

#include "Config.hpp"
#include "Elpida/TaskBatch.hpp"
#include "Elpida/Exceptions/ElpidaException.hpp"
#include "Elpida/PluginLoader.hpp"

#if _elpida_linux
#include <dirent.h>
#define TASK_ENDING ".so"
#elif _elpida_windows
#define TASK_ENDING ".dll"
#include <Windows.h>
#include <strsafe.h>
#endif

namespace Elpida
{

	PluginLoader::PluginLoader()
	{

	}

	void PluginLoader::loadFromFolder(const String& path)
	{
		unloadEverything();
#if _elpida_linux
		DIR *dir;
		dirent *dirent;
		if ((dir = opendir(path.c_str())) != nullptr)
		{
			while ((dirent = readdir(dir)) != nullptr)
			{
				String fileName(dirent->d_name);
				if (fileName.find(TASK_ENDING) != String::npos)
				{
					loadLibraryAndGetTaskBatch(path + "/" + fileName);
				}
			}
			closedir(dir);
		}
		else
		{
			throw ElpidaException("TaskBatchLoader", "'" + path + "' directory could not be opened.");
		}

#elif _elpida_windows
		WIN32_FIND_DATA data;
		HANDLE hFind = FindFirstFile((path + "/*" + TASK_ENDING).c_str(), &data);

		if (hFind != INVALID_HANDLE_VALUE)
		{
			do
			{
				loadLibraryAndGetTaskBatch(path + "/" + data.cFileName);
			}while (FindNextFile(hFind, &data));
			FindClose(hFind);
		}

#endif
	}

	PluginLoader::~PluginLoader()
	{

	}

	void PluginLoader::loadLibraryAndGetTaskBatch(const String& path)
	{
		try
		{
			Plugin plugin = Plugin(path);
			_loadedPlugins.emplace(path, std::move(plugin));
		}
		catch (ElpidaException& e)
		{
			std::cerr << "Failed to load '" + path + "' plugin: " + e.getMessage();
		}
	}

	void PluginLoader::unloadEverything()
	{
		_loadedPlugins.clear();
	}

} /* namespace Elpida */
