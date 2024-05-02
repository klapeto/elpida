//
// Created by klapeto on 14/3/2023.
//

#include "Elpida/Platform/CpuInfoLoader.hpp"

#include "Elpida/Core/Config.hpp"
#include "Elpida/Core/Vector.hpp"

#if defined(__x86_64__) || defined(_M_X64) || defined(WIN32)

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


	static String SanitizeModelName(String& modelName)
	{
		size_t index = modelName.size();
		for (auto itr = modelName.rbegin(); itr != modelName.rend(); ++itr)
		{
			if (iscntrl(*itr) || isspace(*itr))
			{
				index--;
				continue;
			}
			break;
		}

		return modelName.substr(0, index);
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
			"x86_64",
			vendorName,
			SanitizeModelName(modelName)
		};
	}

} // Elpida

#endif