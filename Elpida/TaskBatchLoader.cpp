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
#include <dlfcn.h>
#define TASK_ENDING "Tasks.so"
#elif _elpida_windows
#define TASK_ENDING "Tasks.dll"
#include <Windows.h>
#include <strsafe.h>
#endif

namespace Elpida
{

#if _elpida_windows
	static String GetWindowsError();
#endif

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
		HANDLE hFind = FindFirstFile((path + "/*Tasks.dll").c_str(), &data);      // DIRECTORY

		if (hFind != INVALID_HANDLE_VALUE)
		{
			do
			{
				loadLibraryAndGetTaskBatch(path + "/" + data.cFileName);
			} while (FindNextFile(hFind, &data));
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
		for (auto handle : _loadedHandles)
		{
#if _elpida_linux
			dlclose(handle);
#elif _elpida_windows
			FreeLibrary((HMODULE) handle);
#endif
		}
	}

	void TaskBatchLoader::loadLibraryAndGetTaskBatch(const String& path)
	{
		auto handle =
#if _elpida_linux
		        dlopen(path.c_str(), RTLD_LAZY);
#elif _elpida_windows
		        LoadLibrary(path.c_str());
#endif
		if (handle != nullptr)
		{
			TaskBatch* (*generator)() = (TaskBatch* (*)())
#if _elpida_linux
			dlsym(handle, "createTaskBatch");
#elif _elpida_windows
			GetProcAddress(handle, "createTaskBatch");
#endif
			if (generator != nullptr)
			{
				TaskBatch* batch = generator();
				_loadedObjects.emplace(batch->getName(), batch);
				_loadedHandles.push_back(handle);
			}
			else
			{
				std::cout << "Failed to load: '" << path << "' -> entry function could not be found" << std::endl;
#if _elpida_linux
				dlclose(handle);
#elif _elpida_windows
				FreeLibrary(handle);
#endif

			}
		}
		else
		{
			std::cout << "Failed to load: '" << path << "' -> " <<
#if _elpida_linux
			          dlerror()
#elif _elpida_windows
			          GetWindowsError()
#endif
			          << std::endl;
		}
	}

#if _elpida_windows
	static String GetWindowsError()
	{
		LPVOID lpMsgBuf;
		LPVOID lpDisplayBuf;
		DWORD dw = GetLastError();

		FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		              NULL, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMsgBuf, 0, NULL);

		lpDisplayBuf = (LPVOID) LocalAlloc(LMEM_ZEROINIT,
		                                   (lstrlen((LPCTSTR) lpMsgBuf) + lstrlen((LPCTSTR) "Error") + 40) * sizeof(TCHAR));
		StringCchPrintf((LPTSTR) lpDisplayBuf, LocalSize(lpDisplayBuf) / sizeof(TCHAR), TEXT("%s failed with error %d: %s"), "Error",
		                dw, lpMsgBuf);
		String returnString((const char*)lpDisplayBuf);
		LocalFree(lpMsgBuf);
		LocalFree(lpDisplayBuf);

		return returnString;
	}
#endif

} /* namespace Elpida */
