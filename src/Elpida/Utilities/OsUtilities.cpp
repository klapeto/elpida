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

#include "Elpida/Utilities/ValueUtilities.hpp"
#include "Elpida/ElpidaException.hpp"

#ifdef ELPIDA_WINDOWS
#include <windows.h>
#include <strsafe.h>
#else
#include <sys/utsname.h>
#endif

#include <fstream>

namespace Elpida
{
#ifdef ELPIDA_WINDOWS

	std::string OsUtilities::GetErrorString(HRESULT errorId)
	{
		LPSTR messageBuffer = nullptr;
		try
		{
			if (errorId == 0) return std::string();

			size_t size = FormatMessageA(
					FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL, errorId, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), (LPSTR) & messageBuffer, 0, NULL);

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

	std::string OsUtilities::ReadRegistryKeyFromHKLM(const std::string& subKey, const std::string& key)
	{
		// Modified version of https://stackoverflow.com/a/50821858
		auto regSubKey = Vu::stringToWstring(subKey);
		auto regValue = Vu::stringToWstring(key);
		size_t bufferSize = 0xFFF; // If too small, will be resized down below.
		std::wstring valueBuf; // Contiguous buffer since C++11.
		valueBuf.resize(bufferSize);
		auto cbData = static_cast<DWORD>(bufferSize * sizeof(wchar_t));
		HRESULT rc = RegGetValueW(
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
			return Vu::wstringTostring(valueBuf);
		}
		else
		{
			throw ElpidaException(FUNCTION_NAME, Vu::Cs("Failed to get Registry key: '",subKey, "' and value: '",key,"'. Reason: ",GetErrorString(rc)));
		}
	}

	std::string OsUtilities::GetLastErrorString()
	{
		return GetErrorString(GetLastError());
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
		std::string name;
		std::string version;

		try
		{
			name = OsUtilities::ReadRegistryKeyFromHKLM("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", "ProductName");
		}
		catch (const std::exception&)
		{
			// does not exist?
			name = "Unknown Windows";
		}

		try
		{
			version = OsUtilities::ReadRegistryKeyFromHKLM("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", "ReleaseId");
		}
		catch (const std::exception&)
		{
			// does not exist?
		}

		try
		{
			version  += "." + OsUtilities::ReadRegistryKeyFromHKLM("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", "CurrentBuild");
		}
		catch (const std::exception&)
		{
			// does not exist?
		}

		return { "Windows", name, version};
	}
#endif

	OsInfo OsUtilities::getOsInfo()
	{
		// static, it is pointless to calculate/acquire this info every time
		static OsInfo osInfo = getOsInfoImpl();
		return osInfo;
	}

	void OsUtilities::setCurrentThreadAffinity(unsigned int cpuId)
	{
#ifdef ELPIDA_LINUX
		cpu_set_t mask;
		CPU_ZERO(&mask);
		CPU_SET(cpuId, &mask);
		if (sched_setaffinity(0, sizeof(cpu_set_t), &mask))
		{
			throw ElpidaException("Failed to set thread affinity to: " + std::to_string(cpuId));
		}
#else
		SetThreadAffinityMask(GetCurrentThread(), 1 << (int)cpuId);
#endif
	}

}