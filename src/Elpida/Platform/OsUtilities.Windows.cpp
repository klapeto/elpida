//
// Created by klapeto on 12/3/2023.
//

#include "Elpida/Core/Config.hpp"

#if defined(ELPIDA_WINDOWS)

#include "Elpida/Platform/OsUtilities.hpp"
#include "Elpida/Core/ElpidaException.hpp"

#include <windows.h>
#include <strsafe.h>

namespace Elpida
{

	unsigned int OsUtilities::GetNumaNodeIdForProcessor(unsigned int processorId)
	{
		UCHAR NodeNumber;

		GetNumaProcessorNode((UCHAR)processorId, &NodeNumber);	// TODO: does not work with 64+ processors (thanks windows)
		return NodeNumber;
	}

	String OsUtilities::GetLastErrorString()
	{
		HRESULT errorId = GetLastError();

		if (errorId == 0) return String();

		LPSTR messageBuffer = nullptr;
		try
		{
			size_t size = FormatMessageA(
					FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL, errorId, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), (LPSTR) & messageBuffer, 0, NULL);

			String message(messageBuffer, size);

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
		WCHAR buffer[MAX_PATH];

		auto lenght = GetModuleFileNameW(NULL, buffer, sizeof(buffer));
		if (lenght == MAX_PATH || GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			throw ElpidaException("Failed to get the executable path. Path required larger buffer");
		}

		std::wstring pathStr(buffer, lenght);

		return std::filesystem::path(pathStr);
	}

	std::string OsUtilities::ReadRegistryKeyFromHKLM(const std::string& subKey, const std::string& key)
	{
		// Modified version of https://stackoverflow.com/a/50821858
		auto regSubKey = ValueUtilities::StringToWstring(subKey);
		auto regValue = ValueUtilities::StringToWstring(key);
		size_t bufferSize = 0xFFF; // If too small, will be resized down below.
		std::wstring valueBuf; // Contiguous buffer since C++11.
		valueBuf.resize(bufferSize);
		auto cbData = static_cast<DWORD>(bufferSize * sizeof(wchar_t));

		HKEY hKey = {};
		auto result = RegOpenKeyExW(HKEY_LOCAL_MACHINE, regSubKey.c_str(), 0, KEY_READ, &hKey);

		if (result != ERROR_SUCCESS)
		{
			throw ElpidaException("Failed to get Registry key: '", subKey, "' and value: '", key, "'. Reason: ", OsUtilities::GetLastErrorString());
		}

		result = RegQueryValueExW(hKey,
				regValue.c_str(),
				NULL,
				NULL,
				reinterpret_cast<LPBYTE>(valueBuf.data()),
				&cbData);

		while (result == ERROR_MORE_DATA)
		{
			// Get a buffer that is big enough.
			cbData /= sizeof(wchar_t);
			if (cbData > static_cast<DWORD>(bufferSize))
			{
				bufferSize = static_cast<size_t>(cbData);
			}
			else
			{
				bufferSize *= 2;
				cbData = static_cast<DWORD>(bufferSize * sizeof(wchar_t));
			}
			valueBuf.resize(bufferSize);
			result = RegQueryValueExW(hKey,
					regValue.c_str(),
					NULL,
					NULL,
					reinterpret_cast<LPBYTE>(valueBuf.data()),
					&cbData);
		}
		if (result == ERROR_SUCCESS)
		{
			cbData /= sizeof(wchar_t);
			valueBuf.resize(static_cast<size_t>(cbData - 1)); // remove end null character
			RegCloseKey(hKey);
			return ValueUtilities::WstringTostring(valueBuf);
		}
		else
		{
			RegCloseKey(hKey);
			throw ElpidaException("Failed to get Registry key: '", subKey, "' and value: '", key, "'. Reason: ", OsUtilities::GetLastErrorString());
		}


		RegCloseKey(hKey);
	}

} // Elpida

#endif // defined(ELPIDA_WINDOWS)