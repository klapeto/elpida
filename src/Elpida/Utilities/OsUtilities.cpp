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

//
// Created by klapeto on 21/6/20.
//

#include "Elpida/Utilities/OsUtilities.hpp"

#ifdef ELPIDA_WINDOWS
#include <Windows.h>
#include <strsafe.h>
#else
#include <sys/utsname.h>
#endif

#include <fstream>

namespace Elpida
{
#ifdef ELPIDA_WINDOWS


	std::wstring OsUtilities::ReadRegistryKeyFromHKLM(const std::wstring& regSubKey, const std::wstring& regValue)
	{
		size_t bufferSize = 0xFFF; // If too small, will be resized down below.
		std::wstring valueBuf; // Contiguous buffer since C++11.
		valueBuf.resize(bufferSize);
		auto cbData = static_cast<DWORD>(bufferSize * sizeof(wchar_t));
		auto rc = RegGetValueW(
			HKEY_LOCAL_MACHINE,
			regSubKey.c_str(),
			regValue.c_str(),
			RRF_RT_REG_SZ,
			nullptr,
			static_cast<void*>(valueBuf.data()),
			&cbData
		);
		while (rc == ERROR_MORE_DATA)
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
			rc = RegGetValueW(
				HKEY_LOCAL_MACHINE,
				regSubKey.c_str(),
				regValue.c_str(),
				RRF_RT_REG_SZ,
				nullptr,
				static_cast<void*>(valueBuf.data()),
				&cbData
			);
		}
		if (rc == ERROR_SUCCESS)
		{
			cbData /= sizeof(wchar_t);
			valueBuf.resize(static_cast<size_t>(cbData - 1)); // remove end null character
			return valueBuf;
		}
		else
		{
			throw std::runtime_error("Windows system error code: " + std::to_string(rc));
		}
	}

	std::string OsUtilities::GetLastErrorString()
	{

		LPSTR messageBuffer = nullptr;
		try
		{
			DWORD errorMessageID = GetLastError();
			if (errorMessageID == 0) return std::string();

			size_t size = FormatMessageA(
				FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL), (LPSTR) & messageBuffer, 0, NULL);

			std::string message(messageBuffer, size);

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
#endif

#ifdef ELPIDA_LINUX

	static std::string getVariable(const std::string& line, const std::string& name)
	{
		size_t pos = line.find(name);
		if (pos != std::string::npos)
		{
			pos += name.size();
			if (line[pos] == '\"')
			{
				pos++;
			}
			size_t lastPos = line.find_last_of('\"');
			if (lastPos == std::string::npos)
			{
				lastPos = line.size() - 1;
			}
			return line.substr(pos, lastPos - pos);
		}
		return std::string();
	}

	static OsInfo getOsInfoImpl()
	{
		std::string name;
		std::string category;
		std::string version;
		std::fstream osRelease("/etc/os-release", std::ios::in);
		if (osRelease.good())
		{
			bool nameFound = false;
			bool osVersionFound = false;
			std::string line;
			while ((!nameFound || !osVersionFound) && getline(osRelease, line))
			{
				if (!nameFound)
				{
					name = getVariable(line, "NAME=");
					if (!name.empty())
					{
						nameFound = true;
						continue;
					}
				}
				if (!osVersionFound)
				{
					auto osVersion = getVariable(line, "VERSION=");
					if (!osVersion.empty())
					{
						name += " " + osVersion;
						osVersionFound = true;
						continue;
					}
				}
			}
			osRelease.close();
		}


		utsname unameInfo;
		uname(&unameInfo);

		return { unameInfo.sysname, name, unameInfo.release };
	}
#else
	static OsInfo getOsInfoImpl()
	{
		return { "", "", ""};
	}
#endif


	OsInfo OsUtilities::getOsInfo()
	{
		static OsInfo osInfo = getOsInfoImpl();
		return osInfo;
	}
}