/*
 *  Copyright (c) 2024-2025  Ioannis Panagiotopoulos
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

#include "Elpida/Platform/CpuInfoLoader.hpp"

#include "Elpida/Core/Config.hpp"
#include "Elpida/Core/Vector.hpp"

#include <sstream>

#if defined(__x86_64__) || defined(_M_X64) || defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86)

#ifdef _MSC_VER
#include <intrin.h>
#else

#include <cpuid.h>

#endif

namespace Elpida
{
	static inline void CPUID(unsigned func, unsigned* eax, unsigned* ebx, unsigned* ecx, unsigned* edx)
	{
#ifdef _MSC_VER
		int regs[4] = { -1 };
		__cpuidex(regs, func, *ecx);
		*eax = regs[0];
		*ebx = regs[1];
		*ecx = regs[2];
		*edx = regs[3];
#else
		__get_cpuid_count(func, *ecx, eax, ebx, ecx, edx);
#endif
	}

	static String SanitizeModelName(const String& modelName)
	{
		std::size_t finalChar = modelName.size();
		for (auto itr = modelName.rbegin(); itr != modelName.rend(); ++itr)
		{
			if (iscntrl(*itr) || isspace(*itr))
			{
				finalChar--;
				continue;
			}
			break;
		}

		std::ostringstream stream;

		std::size_t lastSpace = 0;
		std::size_t lastChar = 0;
		for (std::size_t i = 0; i < finalChar; ++i)
		{
			auto c = modelName[i];

			// only single spaces allowed
			if (c == ' ')
			{
				if (((int)(lastChar - lastSpace)) > 0)
				{
					stream << c;
				}
				lastSpace = i;
				continue;
			}

			if (iscntrl(c) || isspace(c))
			{
				continue;
			}
			stream << c;
			lastChar = i;
		}

		return stream.str();
	}

	CpuInfo CpuInfoLoader::Load()
	{
		unsigned eax = 0, ebx = 0, ecx = 0, edx = 0;

		// EAX = 0 => return maximum standard function on EAX and vendor information on EBX,EDX,ECX
		CPUID(0x0, &eax, &ebx, &ecx, &edx);

		String vendorName;

		vendorName.append((char*)&ebx, 4);
		vendorName.append((char*)&edx, 4);
		vendorName.append((char*)&ecx, 4);

		String modelName;
		for (int i = 2; i <= 4; ++i)
		{
			eax = ebx = ecx = edx = 0;
			CPUID(0x80000000 + i, &eax, &ebx, &ecx, &edx);
			modelName.append((char*)(&eax), 4);
			modelName.append((char*)(&ebx), 4);
			modelName.append((char*)(&ecx), 4);
			modelName.append((char*)(&edx), 4);
		}

		return {
#if defined(__x86_64__) || defined(_M_X64)
				"x86_64",
#else
				"i686",
#endif
				vendorName,
				SanitizeModelName(modelName)
		};
	}

} // Elpida

#endif