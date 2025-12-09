/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

//
// Created by klapeto on 14/3/2023.
//
#include "Elpida/Core/Config.hpp"

#if defined(ELPIDA_UNIX) && defined (__riscv_xlen) && (__riscv_xlen == 64)

#include "Elpida/Platform/CpuInfoLoader.hpp"
#include "Elpida/Core/String.hpp"
#include "Elpida/Core/Size.hpp"
#include "Elpida/Core/Map.hpp"

#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <functional>

#include <sys/utsname.h>

#include "jep106.h"

namespace Elpida
{
    CpuInfo CpuInfoLoader::Load()
    {
        std::fstream cpuInfo("/proc/cpuinfo", std::ios::in);

        uint64_t vendorId = 0;
        auto vendorDelegate = [&vendorId](const String& value)
        {
            vendorId = std::strtoul(value.data(), nullptr, 16);
        };

        uint64_t archId = 0;
        auto archIdDelegate = [&archId](const String& value)
        {
            archId = strtol(value.data(), nullptr, 16);
        };

        uint64_t implId = 0;
        auto implIdDelegate = [&implId](const String& value)
        {
            implId = strtol(value.data(), nullptr, 16);
        };

        Map<String, std::function<void(const String&)>> fields
        {
            {"mvendorid", vendorDelegate},
            {"marchid", archIdDelegate},
            {"mimpid", implIdDelegate},
        };


        if (cpuInfo.good())
        {
            String line;
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

        // updated from https://github.com/riscv/riscv-isa-manual/blob/main/marchid.md
        Map<uint64_t, String> openSourceArchIds
        {
            {1, "Rocket"},
            {2, "BOOM"},
            {3, "CVA6"},
            {4, "CV32E40P"},
            {5, "Spike"},
            {6, "E-Class"},
            {7, "ORCA"},
            {8, "SCR1"},
            {9, "YARVI"},
            {10, "RVBS"},
            {11, "SweRV EH1"},
            {12, "MSCC"},
            {13, "BlackParrot"},
            {14, "BaseJump Manycore"},
            {15, "C-Class"},
            {16, "SweRV EL2"},
            {17, "SweRV EH2"},
            {18, "SERV"},
            {19, "NEORV32"},
            {20, "CV32E40X"},
            {21, "CV32E40S"},
            {22, "Ibex"},
            {23, "RudolV"},
            {24, "Steel Core"},
            {25, "XiangShan"},
            {26, "Hummingbirdv2 E203"},
            {27, "Hazard3"},
            {28, "CV32E41P"},
            {29, "Rift"},
            {30, "RISu064"},
            {31, "AIRISC"},
            {32, "Proteus"},
            {33, "VexRiscv"},
            {34, "Shuttle"},
            {35, "CV32E2"},
            {36, "Wally"},
            {37, "Boa32"},
            {38, "WIV64"},
            {39, "RV6"},
            {40, "ApogeoRV"},
            {41, "MicroRV32"},
            {42, "QEMU"},
            {43, "KianV"},
            {44, "Coreblocks"},
            {45, "rrv32"},
            {46, "VexiiRiscv"},
            {47, "Wildcat"},
            {48, "CVA5"},
        };

        String architecture;
        utsname utsnam{};
        if (uname(&utsnam) != -1)
        {
            architecture = utsnam.machine;
        }
        else
        {
            architecture = "riscv64";
        }

        String vendorName;
        String modelName;
        {
            if (vendorId > 0)
            {
                vendorName = jep106_manufacturer(vendorId);
            }
            else
            {
                vendorName = "Unknown";
            }

            if ((archId & (1 << 64)) != 0)
            {
                // commercial arch id
                modelName = std::to_string(archId & 0x7fffffffffffffff);
            }
            else
            {
                auto itr = openSourceArchIds.find(archId);
                if (itr != openSourceArchIds.end())
                {
                    modelName = itr->second;
                }
                else
                {
                    modelName = std::to_string(archId);
                }
            }
        }

        return {
            architecture,
            vendorName,
            modelName
        };
    }
}

#endif
