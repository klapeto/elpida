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
#include <numa.h>
#include <cstring>
#include <cerrno>
#endif

#include <fstream>

namespace Elpida
{
#if defined(ELPIDA_WINDOWS)
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
			throw ElpidaException(FUNCTION_NAME, Vu::Cs("Failed to get Registry key: '",subKey, "' and value: '",key,"'. Reason: ", GetErrorString(rc)));
		}
	}
#endif

	std::string OsUtilities::GetLastErrorString()
	{
#if defined(ELPIDA_LINUX)
		return std::string(strerror(errno));
#elif defined(ELPIDA_WINDOWS)
		return GetErrorString(GetLastError());
#else
#error "Unsupported Platform"
#endif
	}

#if defined(ELPIDA_LINUX)

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
#elif defined(ELPIDA_WINDOWS)
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
#else
#error "Unsupported Platform"
#endif

	OsInfo OsUtilities::getOsInfo()
	{
		// static, it is pointless to calculate/acquire this info every time
		static OsInfo osInfo = getOsInfoImpl();
		return osInfo;
	}

	void OsUtilities::setCurrentThreadAffinity(unsigned int cpuId)
	{
		bool fail = false;
#if defined(ELPIDA_LINUX)
		cpu_set_t mask;
		CPU_ZERO(&mask);
		CPU_SET(cpuId, &mask);

		fail = sched_setaffinity(0, sizeof(cpu_set_t), &mask);
#elif defined(ELPIDA_WINDOWS)
		fail = !SetThreadAffinityMask(GetCurrentThread(), 1 << (int)cpuId);
#else
#error "Unsupported Platform"
#endif

		if (fail)
		{
			throw ElpidaException(FUNCTION_NAME, Vu::Cs("Failed to set thread affinity to: " ,cpuId, ". Error: ", GetLastErrorString()));
		}
	}
	void OsUtilities::deallocateOnNumaNode(void* data, size_t size)
	{
#if defined(ELPIDA_LINUX)
		numa_free(data, size);
#elif defined(ELPIDA_WINDOWS)
		VirtualFree(data, 0, MEM_RELEASE);
#else
#error "Unsupported Platform"
#endif
	}

	void* OsUtilities::allocateOnNumaNode(size_t size, int numaNode)
	{
		void* ptr;
#if defined(ELPIDA_LINUX)
		if (numa_available() < 0)
		{
			throw ElpidaException(FUNCTION_NAME, "Numa API is not available!");
		}
		numa_set_strict(1);
		ptr = (void*)numa_alloc_onnode(size, numaNode);
#elif defined(ELPIDA_WINDOWS)
		ptr = (void*)VirtualAllocExNuma(
			GetCurrentProcess(),
			NULL,
			size,
			MEM_RESERVE | MEM_COMMIT,
			PAGE_READWRITE,
			(UCHAR)numaNode
		);
#else
#error "Unsupported Platform"
#endif
		if  (ptr == nullptr)
		{
			throw ElpidaException(FUNCTION_NAME, Vu::Cs("Failed to allocate to numa node " , numaNode, ". Memory of size: ", Vu::getValueScaleStringIEC(size), "B. Error: ", GetLastErrorString()));
		}

		return ptr;
	}

}
