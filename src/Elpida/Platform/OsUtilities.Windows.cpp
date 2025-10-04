/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

//
// Created by klapeto on 12/3/2023.
//

#include "Elpida/Core/Config.hpp"

#if defined(ELPIDA_WINDOWS)

#include "Elpida/Platform/OsUtilities.hpp"
#include "Elpida/Core/ElpidaException.hpp"
#include "Elpida/Core/ValueUtilities.hpp"

#include <windows.h>
#include <strsafe.h>

namespace Elpida
{
	unsigned int OsUtilities::GetNumaNodeIdForProcessor(unsigned int processorId)
	{
		UCHAR NodeNumber;

		GetNumaProcessorNode(static_cast<UCHAR>(processorId), &NodeNumber);	// TODO: does not work with 64+ processors (thanks windows)
		return NodeNumber;
	}

	bool OsUtilities::ConvertArgumentsToUTF8(int& originalArgC, char**& originalArgV)
	{
		// based on https://github.com/HandBrake/HandBrake/blame/master/test/test.c
#if defined( __MINGW32__ )
		bool ret = false;
		int argc = 0;
		char **argv = nullptr;

		auto **argVUtf16 = CommandLineToArgvW(GetCommandLineW(), &argc);
		if (argVUtf16 != nullptr)
		{
			int offset = (argc + 1) * sizeof(char*);
			int size = offset;

			for(auto i = 0; i < argc; i++)
			{
				size += WideCharToMultiByte(CP_UTF8, 0, argVUtf16[i], -1, nullptr, 0, nullptr, nullptr );
			}

			argv = static_cast<char **>(malloc(size));
			if (argv != nullptr)
			{
				for (auto i = 0; i < argc; i++)
				{
					argv[i] = reinterpret_cast<char *>(argv) + offset;
					offset += WideCharToMultiByte(CP_UTF8, 0, argVUtf16[i], -1, argv[i], size - offset, nullptr, nullptr);
				}
				argv[argc] = nullptr;
				ret = true;
			}
			LocalFree(argVUtf16);
		}
		if (ret)
		{
			originalArgC = argc;
			originalArgV = argv;
		}
		return ret;
#else
		// On other systems, assume command line is already utf8
		return true;
#endif
	}

	std::string OsUtilities::GetStringFromWinApiBuffer(LPCTSTR buffer, DWORD size) {

#if UNICODE
		return ValueUtilities::WstringTostring({ buffer, static_cast<std::size_t>(size) });
#else
		return { reinterpret_cast<const char*>(buffer), static_cast<std::size_t>(size)};
#endif
	}

#if UNICODE
std::wstring
#else
std::string
#endif
	OsUtilities::GetWinApiStringFromString(const std::string &str) {
#if UNICODE
		return ValueUtilities::StringToWstring(str);
#else
		return str;
#endif
	}

	std::string OsUtilities::GetLastErrorString()
	{
		const DWORD errorId = GetLastError();

		if (errorId == 0) return {};

		LPTSTR messageBuffer = nullptr;
		try
		{
			const auto size = FormatMessage(
					FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
					nullptr,
					errorId,
					MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
					reinterpret_cast<LPTSTR>(&messageBuffer),
					0,
					nullptr);

			if (size == 0) return ValueUtilities::Cs("Error getting string of the error: ", std::to_string(GetLastError()) , " (original error: " ,std::to_string(errorId), ")");

			std::string message = GetStringFromWinApiBuffer(messageBuffer, size);
			LocalFree(messageBuffer);
			return message;
		}
		catch (...)
		{
			if (messageBuffer != nullptr)
			{
				LocalFree(messageBuffer);
			}

			throw;
		}
	}

	std::filesystem::path OsUtilities::GetExecutableDirectory()
	{
		return GetExecutablePath().parent_path();
	}

	std::filesystem::path OsUtilities::GetExecutablePath()
	{
		TCHAR buffer[MAX_PATH];

		const auto length = GetModuleFileName(NULL, buffer, sizeof(buffer));
		if (length == MAX_PATH || GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			throw ElpidaException("Failed to get the executable path. Path required larger buffer");
		}

		std::string pathStr = GetStringFromWinApiBuffer(buffer, length);

		return std::filesystem::path(pathStr);
	}

	std::string OsUtilities::ReadRegistryKeyFromHKLM(const std::string& subKey, const std::string& key)
	{
		// Modified version of https://stackoverflow.com/a/50821858
		const auto regSubKey = GetWinApiStringFromString(subKey);
		const auto regValue = GetWinApiStringFromString(key);
		size_t bufferSize = 0xFFF; // If too small, will be resized down below.
		std::wstring valueBuf; // Contiguous buffer since C++11.
		valueBuf.resize(bufferSize);
		auto cbData = static_cast<DWORD>(bufferSize * sizeof(TCHAR));

		HKEY hKey = {};
		auto result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, regSubKey.c_str(), 0, KEY_READ, &hKey);

		if (result != ERROR_SUCCESS)
		{
			throw ElpidaException("Failed to get Registry key: '", subKey, "' and value: '", key, "'. Reason: ", GetLastErrorString());
		}

		result = RegQueryValueEx(hKey,
				regValue.c_str(),
				nullptr,
				nullptr,
				reinterpret_cast<LPBYTE>(valueBuf.data()),
				&cbData);

		while (result == ERROR_MORE_DATA)
		{
			// Get a buffer that is big enough.
			cbData /= sizeof(TCHAR);
			if (cbData > static_cast<DWORD>(bufferSize))
			{
				bufferSize = static_cast<size_t>(cbData);
			}
			else
			{
				bufferSize *= 2;
				cbData = static_cast<DWORD>(bufferSize * sizeof(TCHAR));
			}
			valueBuf.resize(bufferSize);
			result = RegQueryValueEx(hKey,
					regValue.c_str(),
					nullptr,
					nullptr,
					reinterpret_cast<LPBYTE>(valueBuf.data()),
					&cbData);
		}
		if (result == ERROR_SUCCESS)
		{
			cbData /= sizeof(TCHAR);
			valueBuf.resize(static_cast<size_t>(cbData - 1)); // remove end null character
			RegCloseKey(hKey);
			return ValueUtilities::WstringTostring(valueBuf);
		}
		RegCloseKey(hKey);
		throw ElpidaException("Failed to get Registry key: '", subKey, "' and value: '", key, "'. Reason: ", OsUtilities::GetLastErrorString());
	}

} // Elpida

#endif // defined(ELPIDA_WINDOWS)