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
 * SharedLibrary.cpp
 *
 *  Created on: 26 Σεπ 2018
 *      Author: klapeto
 */

#include "Elpida/SharedLibrary.hpp"

#include "Config.hpp"
#include "Elpida/Exceptions/ElpidaException.hpp"

#if _elpida_linux
#include <dirent.h>
#include <dlfcn.h>
#elif _elpida_windows
#include <Windows.h>
#include <strsafe.h>
#include <string>
#endif

namespace Elpida
{
#if _elpida_windows
	static std::string GetWindowsError();
#endif

	SharedLibrary::SharedLibrary(const std::string& libraryPath)
	{
		_handle =
#if _elpida_linux
		        dlopen(libraryPath.c_str(), RTLD_LAZY);
#elif _elpida_windows
		LoadLibrary(libraryPath.c_str());
#endif
		if (_handle == nullptr)
		{
#if _elpida_linux
			auto errorMessage = dlerror();
#endif
			throw ElpidaException("Plugin", "Error loading plugin: '" + libraryPath + "' -> " +
#if _elpida_linux
			                              (errorMessage != nullptr ? std::string(errorMessage) : std::string("(Unknown error)")));
#elif _elpida_windows
			GetWindowsError());
#endif
		}
	}

	SharedLibrary::~SharedLibrary()
	{
		if (_handle != nullptr)
		{
#if _elpida_linux
			dlclose(_handle);
#elif _elpida_windows
			FreeLibrary((HMODULE) _handle);
#endif
		}
	}

	void* SharedLibrary::getFunctionPointerImpl(const std::string& functionName) const
	{
		return _handle != nullptr ?
#if _elpida_linux
		                            dlsym(_handle, functionName.c_str())
#elif _elpida_windows
		                                  (void*) GetProcAddress((HMODULE)_handle, functionName.c_str())
#endif
		                                  :
		                            nullptr;
	}

#if _elpida_windows
static std::string GetWindowsError()
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
	std::string returnString((const char*)lpDisplayBuf);
	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);

	return returnString;
}
#endif

}
/* namespace Elpida */
