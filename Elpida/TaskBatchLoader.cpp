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

#include "Elpida/TaskBatchLoader.hpp"

#include <iostream>

#include "Config.hpp"
#include "Elpida/TaskBatch.hpp"
#include "Elpida/Exceptions/ElpidaException.hpp"

#if _elpida_linux
#include <dirent.h>
#define TASK_ENDING "Tasks.so"
#elif _elpida_windows
#define TASK_ENDING "Tasks.dll"
#include <Windows.h>
#include <strsafe.h>
#endif

namespace Elpida
{

	TaskBatchLoader::TaskBatchLoader()
	{

	}

	TaskBatch& TaskBatchLoader::getBatch(const String& name)
	{
		auto itr = _loadedObjects.find(name);
		if (itr != _loadedObjects.end())
		{
			return *itr->second;
		}
		else
		{
			throw ElpidaException("TaskBatchLoader", "'" + name + "' batch was not found or not loaded.");
		}
	}

	void TaskBatchLoader::loadFromFolder(const String& path)
	{
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
		HANDLE hFind = FindFirstFile((path + "/*Tasks.dll").c_str(), &data);

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

	TaskBatchLoader::~TaskBatchLoader()
	{
		for (auto object : _loadedObjects)
		{
			delete object.second;
		}
	}

	void TaskBatchLoader::loadLibraryAndGetTaskBatch(const String& path)
	{
		try
		{
			Plugin plugin = Plugin(path);
			auto taskBatchCreator = plugin.getFunctionPointer<TaskBatch* (*)()>("createTaskBatch");
			if (taskBatchCreator != nullptr)
			{
				TaskBatch* taskBatch = taskBatchCreator();
				_loadedObjects.emplace(taskBatch->getName(), taskBatch);
				_loadedPlugins.push_back(std::move(plugin));
			}
			else
			{
				std::cerr << "Failed to get function 'createTaskBatch' from '" + path + "' plugin";
			}
		}
		catch (ElpidaException& e)
		{
			std::cerr << "Failed to load '" + path + "' plugin: " + e.getMessage();
		}
	}

} /* namespace Elpida */
