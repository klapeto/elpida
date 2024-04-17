//
// Created by klapeto on 13/3/2023.
//

#include "Elpida/Core/Config.hpp"

#if defined(ELPIDA_WINDOWS)

#include "Elpida/Platform/OsInfoLoader.hpp"
#include "Elpida/Platform/OsUtilities.hpp"
#include "Elpida/Core/ValueUtilities.hpp"
#include "Elpida/Core/ElpidaException.hpp"

#include <windows.h>

namespace Elpida
{
	String ReadRegistryKeyFromHKLM(const String& subKey, const String& key)
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

	OsInfo OsInfoLoader::Load()
	{
		String name;
		try
		{
			name = ReadRegistryKeyFromHKLM(R"(SOFTWARE\Microsoft\Windows NT\CurrentVersion)", "ProductName");
		}
		catch (const std::exception&)
		{
			// does not exist?
			name = "Unknown Windows";
		}

		String version;
		try
		{
			version = ReadRegistryKeyFromHKLM(R"(SOFTWARE\Microsoft\Windows NT\CurrentVersion)", "ReleaseId");
		}
		catch (const std::exception&)
		{
			version = "Unknown version";
		}

		try
		{
			version  += "." + ReadRegistryKeyFromHKLM(R"(SOFTWARE\Microsoft\Windows NT\CurrentVersion)", "CurrentBuild");
		}
		catch (const std::exception&)
		{
			version = "Unknown Build";
		}

		return {
			"Windows",
			name,
			version
		};
	}
} // Elpida

#endif // defined(ELPIDA_WINDOWS)