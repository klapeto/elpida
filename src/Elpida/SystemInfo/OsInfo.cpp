/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2021  Ioannis Panagiotopoulos
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
// Created by klapeto on 30/1/21.
//

#include "Elpida/SystemInfo/OsInfo.hpp"
#include "Elpida/Config.hpp"
#include "Elpida/Utilities/ValueUtilities.hpp"


#ifdef ELPIDA_WINDOWS
#include <windows.h>
#include <strsafe.h>
#include "Elpida/Utilities/OsUtilities.hpp"
#else
#include <sys/utsname.h>
#include <cstring>
#endif

#include <fstream>

namespace Elpida
{
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
#endif

	void OsInfo::getOsInfoImpl()
	{
#if defined(ELPIDA_LINUX)
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
					_name = getVariable(line, "NAME=");
					if (!_name.empty())
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
						_name += " " + osVersion;
						osVersionFound = true;
						continue;
					}
				}
			}
			osRelease.close();
		}

		utsname unameInfo;
		uname(&unameInfo);

		_category = unameInfo.sysname;
		_version = unameInfo.release;
#elif defined(ELPIDA_WINDOWS)

		try
		{
			_name = OsUtilities::ReadRegistryKeyFromHKLM("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", "ProductName");
		}
		catch (const std::exception&)
		{
			// does not exist?
			_name = "Unknown Windows";
		}

		try
		{
			_version = OsUtilities::ReadRegistryKeyFromHKLM("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", "ReleaseId");
		}
		catch (const std::exception&)
		{
			// does not exist?
		}

		try
		{
			_version  += "." + OsUtilities::ReadRegistryKeyFromHKLM("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", "CurrentBuild");
		}
		catch (const std::exception&)
		{
			// does not exist?
		}

		_category = "Windows";

#else
#error "Unsupported Platform"
#endif
	}

	OsInfo::OsInfo()
	{
		getOsInfoImpl();
	}
}