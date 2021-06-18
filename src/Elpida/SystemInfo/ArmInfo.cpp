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
#include "Elpida/Utilities/OsUtilities.hpp"

#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <functional>
#include <filesystem>

#include <sys/utsname.h>

namespace Elpida
{

	static size_t parseSize(const std::string& value)
	{
		if (!value.empty())
		{
			auto actualValue = strtol(value.data(), nullptr, 10);

			auto suffix = value.back();
			if (!isdigit(suffix))
			{
				switch (suffix)
				{
				case 'K':
					return actualValue * 1000;
				case 'M':
					return actualValue * 1, 000, 000;
				case 'G':
					return actualValue * 1, 000, 000, 000;
				case 'T':
					return actualValue * 1, 000, 000, 000, 000;
				default:
					return actualValue * 1, 000, 000, 000, 000, 000;
				}
			}
			return actualValue;
		}
		else
		{
			return 0;
		}
	}

	CpuInfo::CpuInfo()
	{
#if defined(ELPIDA_LINUX)

		try
		{
			_frequency = OsUtilities::readSysfsValue("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq") * 1000;
		}
		catch (...)
		{
			_frequency = 0.0;
		}

		try
		{
			for (auto& dir: std::filesystem::directory_iterator("/sys/devices/system/cpu/cpu0/cache/"))
			{
				if (dir.is_directory())
				{
					auto path = dir.path().string();

					auto level = (size_t)OsUtilities::readSysfsValue(std::filesystem::path(path) / "level");
					auto type = OsUtilities::readSysfsString(std::filesystem::path(path) / "type");
					auto size = OsUtilities::readSysfsString(std::filesystem::path(path) / "size");
					auto associativity = OsUtilities::readSysfsString(
							std::filesystem::path(path) / "ways_of_associativity");
					auto lineSize = OsUtilities::readSysfsValue(
							std::filesystem::path(path) / "coherency_line_size");

					_caches.emplace_back(level,
							"L" + std::to_string(level) + " " + type,
							associativity + " way",
							parseSize(size),
							lineSize);
				}
			}
		}
		catch (...)
		{
			// TODO: Log
		}

		std::fstream cpuInfo("/proc/cpuinfo", std::ios::in);

		auto featuresDelegate = [this](const std::string& value)
		{
			std::stringstream inStream(value);
			std::string token;
			while (getline(inStream, token, ' '))
			{
				if (!token.empty())
				{
					_features.emplace_back(token, "", true);
				}
			}
		};


		size_t cpuImplementer = 0;

		auto vendorDelegate = [&cpuImplementer](const std::string& value)
		{
			cpuImplementer = strtol(value.data(), nullptr, 16);;
		};

		auto modelIdDelegate = [this](const std::string& value)
		{
			_modelId = strtol(value.data(), nullptr, 16);
		};

		std::unordered_map<std::string, std::function<void(const std::string&)>> fields
				{
						{ "Features",         featuresDelegate },
//						{ "type",             featuresDelegate },
//						{ "flags",            featuresDelegate },
//						{ "features",         featuresDelegate },
//						{ "ASEs implemented", featuresDelegate },

						{ "CPU implementer",  vendorDelegate },
//						{ "vendor",           vendorDelegate },
//						{ "vendor_id",        vendorDelegate },

//						{ "cpu",              modelNameDelegate },
//						{ "model name",       modelNameDelegate },
//
//						{ "model",            modelIdDelegate },
//						{ "cpu model",        modelIdDelegate },
						{ "CPU part",         modelIdDelegate }
				};

		if (cpuInfo.good())
		{
			std::string line;
			while (getline(cpuInfo, line))
			{
				if (line.empty()) break;
				auto fieldName = line.substr(0, line.find('\t'));
				auto itr = fields.find(fieldName);
				if (itr != fields.end())
				{
					size_t i = line.find(':');

					while (i < line.size()
						   && (iswspace(line[i]) || line[i] == ':'))
					{
						i++;
					}

					if (i < line.size())
					{
						itr->second(line.substr(i));
					}
				}
			}
			cpuInfo.close();
		}

		std::unordered_map<size_t, std::tuple<std::string, std::unordered_map<size_t, std::string>>> modelMaps
				{
						{ 0x41, {"ARM",
													  {
															  { 0x810, "ARM810" },
															  { 0x920, "ARM920" },
															  { 0x922, "ARM922" },
															  { 0x926, "ARM926" },
															  { 0x940, "ARM940" },
															  { 0x946, "ARM946" },
															  { 0x966, "ARM966" },
															  { 0xa20, "ARM1020" },
															  { 0xa22, "ARM1022" },
															  { 0xa26, "ARM1026" },
															  { 0xb02, "ARM11 MPCore" },
															  { 0xb36, "ARM1136" },
															  { 0xb56, "ARM1156" },
															  { 0xb76, "ARM1176" },
															  { 0xc05, "Cortex-A5" },
															  { 0xc07, "Cortex-A7" },
															  { 0xc08, "Cortex-A8" },
															  { 0xc09, "Cortex-A9" },
															  { 0xc0d, "Cortex-A17" },
															  { 0xc0f, "Cortex-A15" },
															  { 0xc0e, "Cortex-A17" },
															  { 0xc14, "Cortex-R4" },
															  { 0xc15, "Cortex-R5" },
															  { 0xc17, "Cortex-R7" },
															  { 0xc18, "Cortex-R8" },
															  { 0xc20, "Cortex-M0" },
															  { 0xc21, "Cortex-M1" },
															  { 0xc23, "Cortex-M3" },
															  { 0xc24, "Cortex-M4" },
															  { 0xc27, "Cortex-M7" },
															  { 0xc60, "Cortex-M0+" },
															  { 0xd01, "Cortex-A32" },
															  { 0xd03, "Cortex-A53" },
															  { 0xd04, "Cortex-A35" },
															  { 0xd05, "Cortex-A55" },
															  { 0xd06, "Cortex-A65" },
															  { 0xd07, "Cortex-A57" },
															  { 0xd08, "Cortex-A72" },
															  { 0xd09, "Cortex-A73" },
															  { 0xd0a, "Cortex-A75" },
															  { 0xd0b, "Cortex-A76" },
															  { 0xd0c, "Neoverse-N1" },
															  { 0xd0d, "Cortex-A77" },
															  { 0xd0e, "Cortex-A76AE" },
															  { 0xd13, "Cortex-R52" },
															  { 0xd20, "Cortex-M23" },
															  { 0xd21, "Cortex-M33" },
															  { 0xd41, "Cortex-A78" },
															  { 0xd42, "Cortex-A78AE" },
															  { 0xd4a, "Neoverse-E1" },
															  { 0xd4b, "Cortex-A78C" }
													  }
								}
						},
						{ 0x42, {"Broadcom",
													  {
															  { 0x0f,  "Brahma B15" },
															  { 0x100, "Brahma B53" },
															  { 0x516, "ThunderX2" }
													  }
								}
						},
						{ 0x43, {"Cavium",
													  {
															  { 0x0a0, "ThunderX" },
															  { 0x0a1, "ThunderX 88XX" },
															  { 0x0a2, "ThunderX 81XX" },
															  { 0x0a3, "ThunderX 83XX" },
															  { 0x0af, "ThunderX2 99xx" }
													  }
								}
						},
						{ 0x44, {"DEC",
													  {
															  { 0xa10, "SA110" },
															  { 0xa11, "SA1100" }
													  }
								}
						},
						{ 0x46, {"FUJITSU",
													  {
															  { 0x001, "A64FX" }
													  }
								}
						},
						{ 0x48, {"HiSilicon",
													  {
															  { 0xd01, "Kunpeng-920" }
													  }
								}
						},
						{ 0x49, {"Infineon",{}}},
						{ 0x4d, {"Motorola/Freescale",{}}},
						{ 0x4e, {"NVIDIA",
													  {
															  { 0x000, "Denver" },
															  { 0x003, "Denver 2" },
															  { 0x004, "Carmel" }
													  }
								}
						},
						{ 0x50, {"APM",
													  {
															  { 0x000, "X-Gene" }
													  }
								}
						},
						{ 0x51, {"Qualcomm",
													  {
															  { 0x00f, "Scorpion" },
															  { 0x02d, "Scorpion" },
															  { 0x04d, "Krait" },
															  { 0x06f, "Krait" },
															  { 0x201, "Kryo" },
															  { 0x205, "Kryo" },
															  { 0x211, "Kryo" },
															  { 0x800, "Falkor V1/Kryo" },
															  { 0x801, "Kryo V2" },
															  { 0xc00, "Falkor" },
															  { 0xc01, "Saphira" }
													  }
								}
						},
						{ 0x53, {"Samsung",
													  {
															  { 0x001, "exynos-m1" }
													  }
								}
						},
						{ 0x56, {"Marvell",
													  {
															  { 0x131, "Feroceon 88FR131" },
															  { 0x581, "PJ4/PJ4b" },
															  { 0x584, "PJ4B-MP" }
													  }
								}
						},
						{ 0x61,{"Apple",{}}},
						{ 0x66, {"Faraday",
													  {
															  { 0x526, "FA526" },
															  { 0x626, "FA626" }
													  }
								}
						},
						{ 0x69, {"Intel",
													  {
															  { 0x200, "i80200" },
															  { 0x210, "PXA250A" },
															  { 0x212, "PXA210A" },
															  { 0x242, "i80321-400" },
															  { 0x243, "i80321-600" },
															  { 0x290, "PXA250B/PXA26x" },
															  { 0x292, "PXA210B" },
															  { 0x2c2, "i80321-400-B0" },
															  { 0x2c3, "i80321-600-B0" },
															  { 0x2d0, "PXA250C/PXA255/PXA26x" },
															  { 0x2d2, "PXA210C" },
															  { 0x411, "PXA27x" },
															  { 0x41c, "IPX425-533" },
															  { 0x41d, "IPX425-400" },
															  { 0x41f, "IPX425-266" },
															  { 0x682, "PXA32x" },
															  { 0x683, "PXA930/PXA935" },
															  { 0x688, "PXA30x" },
															  { 0x689, "PXA31x" },
															  { 0xb11, "SA1110" },
															  { 0xc12, "IPX1200" }
													  }
								}
						},
						{ 0xc0, {"Ampere",{}}}
				};



		utsname utsnam{};
		if (uname(&utsnam) != -1)
		{
			_architecture = utsnam.machine;
		}
		else
		{
#if defined(__aarch64__)
			_architecture = "AArch64";
#else
			_architecture = "ARM";
#endif
		}

		{
			auto vendorItr = modelMaps.find(cpuImplementer);
			if (vendorItr != modelMaps.end())
			{
				auto [ vendorName, models ] = vendorItr->second;
				_vendorName = vendorName;
				auto modelItr = models.find(_modelId);
				if (modelItr != models.end())
				{
					_modelName = modelItr->second;
				}
			}
			else
			{
				_vendorName = "Unknown vendor";
				_modelName = "Generic Arm processor";
			}
		}
#else
#error "Unsupported operating system"
#endif
	}
}

#endif