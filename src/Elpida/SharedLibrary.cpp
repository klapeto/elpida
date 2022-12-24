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
 * SharedLibrary.cpp
 *
 *  Created on: 26 Σεπ 2018
 *      Author: klapeto
 */

#include "Elpida/SharedLibrary.hpp"

#include "Elpida/Config.hpp"
#include "Elpida/ElpidaException.hpp"

#ifdef ELPIDA_LINUX

#include <dirent.h>
#include <dlfcn.h>

#else
#include <windows.h>
#include <strsafe.h>
#include <string>
#include "Elpida/Utilities/OsUtilities.hpp"
#endif

namespace Elpida
{

	SharedLibrary::SharedLibrary(SharedLibrary&& other) noexcept
	{
		this->_handle = other._handle;
		other._handle = nullptr;
	}

	SharedLibrary& SharedLibrary::operator=(SharedLibrary&& other) noexcept
	{
		this->_handle = other._handle;
		other._handle = nullptr;
		return *this;
	}

	SharedLibrary::SharedLibrary(const std::string& libraryPath)
			: _path(libraryPath)
	{
		_handle =
#ifdef ELPIDA_LINUX
				dlopen(libraryPath.c_str(), RTLD_LAZY);
#else
		LoadLibrary(libraryPath.c_str());
#endif
		if (_handle == nullptr)
		{
#ifdef ELPIDA_LINUX
			auto errorMessage = dlerror();
#endif
			throw ElpidaException("Plugin", "Error loading plugin: '" + libraryPath + "' -> " +
											#ifdef ELPIDA_LINUX
											(errorMessage != nullptr ? std::string(errorMessage) : std::string(
													"(Unknown error)")));
#else
			OsUtilities::GetLastErrorString());
#endif
		}
	}

	SharedLibrary::~SharedLibrary()
	{
		if (_handle != nullptr)
		{
#ifdef ELPIDA_LINUX
			dlclose(_handle);
#else
			FreeLibrary((HMODULE) _handle);
#endif
		}
	}

	void* SharedLibrary::getFunctionPointerImpl(const std::string& functionName) const
	{
		return _handle != nullptr ?
			   #ifdef ELPIDA_LINUX
			   dlsym(_handle, functionName.c_str())
			   #else
			   (void*) GetProcAddress((HMODULE)_handle, functionName.c_str())
			   #endif
								  :
			   nullptr;
	}

}
/* namespace Elpida */
