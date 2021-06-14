/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2021 Ioannis Panagiotopoulos
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
// Created by klapeto on 13/6/21.
//

#if defined(__aarch64__) || defined(_M_ARM) || defined(__arm__)

#include "Elpida/SystemInfo/CpuInfo.hpp"
#include "Elpida/Config.hpp"
#include "Elpida/SystemInfo/CpuFeature.hpp"

#include <sstream>
#include <unordered_map>
#include <functional>
#include <fstream>

namespace Elpida
{

	CpuInfo::CpuInfo()
	{
#if defined(ELPIDA_LINUX)
		std::fstream cpuInfo("/proc/cpuinfo", std::ios::in);

		std::unordered_map<std::string, std::function<void(const std::string&)>> fields
				{
						{ "Features",
								[this](const std::string& value)
								{
									std::stringstream inStream(value);
									std::string token;
									while (getline(inStream, token, ' '))
									{
										if (token.size() > 0)
										{
											_features.emplace_back(token, "", true);
										}
									}
								}
						},
						{ "Hardware",
								[this](const std::string& value)
								{
									_vendorString = value.sub;
								}
						},
						{ "Model",
								[this](const std::string& value)
								{
									_processorBrand = value;
								}
						}
				};

		if (cpuInfo.good())
		{
			std::string line;
			while (!fields.empty() && getline(cpuInfo, line))
			{
				auto fieldName = line.substr(0, line.find('\t'));
				auto itr = fields.find(fieldName);
				if (itr != fields.end())
				{
					auto value = line.substr(line.find(':'));
					value = value.substr(value.find('\t'));
					value = value.substr(value.find(' '));
					itr->second(value);
					fields.erase(itr);
				}
			}
			cpuInfo.close();
		}

#if defined(__aarch64__)
		_architecture = "AArch64";
#else
		_architecture = "ARM";
#endif

		if (_vendorString.empty())
		{
			_vendorString = "ARM Cortex";
		}

		if (_processorBrand.empty())
		{
			_processorBrand = "Generic Arm processor";
		}

#else
#error "Unsupported operating system"
#endif
	}
}

#endif