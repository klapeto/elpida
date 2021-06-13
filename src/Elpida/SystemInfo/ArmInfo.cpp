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

#if defined(__aarch64__) || defined(_M_ARM) || defined(__TARGET_ARCH_ARM)

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
						{ "model name",
								[this](const std::string& value)
								{
									_processorBrand = value;
								}
						},
						{ "Features",
								[this](const std::string& value)
								{
									std::stringstream inStream(value);
									std::string token;
									while (getline(inStream, token, ' '))
									{
										_features.emplace_back(token, "", true);
									}
								}
						},
						{ "Cpu architecture",
								[this](const std::string& value)
								{
									_additionalInformation.emplace("Architecture", value);
								}
						},
						{ "Cpu variant",
								[this](const std::string& value)
								{
									_additionalInformation.emplace("Variant", value);
								}
						},
						{ "Cpu part",
								[this](const std::string& value)
								{
									_additionalInformation.emplace("Part", value);
								}
						},
						{ "Cpu revision",
								[this](const std::string& value)
								{
									_additionalInformation.emplace("Revision", value);
								}
						},
						{ "Hardware",
								[this](const std::string& value)
								{
									_additionalInformation.emplace("Hardware name", value);
								}
						},
						{ "Revision",
								[this](const std::string& value)
								{
								}
						},
						{ "Model",
								[this](const std::string& value)
								{
									_vendorString = value;
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
					auto value = line.substr(line.find(':') + 1);
					itr->second(value);
					fields.erase(itr);
				}
			}
			cpuInfo.close();
		}

#else
#error "Unsupported operating system"
#endif
	}
}

#endif