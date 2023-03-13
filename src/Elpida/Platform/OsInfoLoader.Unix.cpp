//
// Created by klapeto on 13/3/2023.
//

#include "Elpida/Core/Config.hpp"

#if defined(ELPIDA_UNIX)

#include "Elpida/Platform/OsInfoLoader.hpp"

#include <sys/utsname.h>
#include <cstring>
#include <fstream>

namespace Elpida
{
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
		return {};
	}

	OsInfo OsInfoLoader::Load()
	{
		String name;
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

		utsname unameInfo{};
		uname(&unameInfo);

		return {
			unameInfo.sysname,
			name,
			unameInfo.release
		};
	}
} // Elpida

#endif // defined(ELPIDA_UNIX)