/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2023  Ioannis Panagiotopoulos
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

//
// Created by klapeto on 25/2/2023.
//

#include "Elpida/SharedLibrary.hpp"

#include "Elpida/Config.hpp"

#if defined(ELPIDA_WINDOWS)
#include <windows.h>
#include <strsafe.h>
#include <string>
#include "Elpida/Utilities/OsUtilities.hpp"
#include "Elpida/ElpidaException.hpp"

namespace Elpida
{
	void* SharedLibrary::loadLibrary(const std::string& libraryPath)
	{
		return LoadLibrary(libraryPath.c_str());
	}

	std::string SharedLibrary::getLoadError()
	{
		return OsUtilities::GetLastErrorString();
	}

	void SharedLibrary::unloadLibrary(void* libraryHandle)
	{
		FreeLibrary((HMODULE)libraryHandle);
	}

	void* SharedLibrary::getFunctionPointerImpl(void* libraryHandle, const std::string& functionName)
	{
		return (void*)GetProcAddress((HMODULE)libraryHandle, functionName.c_str());
	}
}

#endif