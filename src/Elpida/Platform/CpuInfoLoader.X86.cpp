//
// Created by klapeto on 14/3/2023.
//

#include "Elpida/Platform/CpuInfoLoader.hpp"

#include "Elpida/Core/Config.hpp"

#if defined(__x86_64__) || defined(_M_X64) || defined(WIN32)

#ifdef _MSC_VER
#include <intrin.h>
#else
#include <cpuid.h>
#endif

namespace Elpida
{
	static inline const char* Stepping = "Stepping";
	static inline const char* Family = "Family";
	static inline const char* Model = "Model";

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

	static constexpr bool FeatureCheck(unsigned reg, unsigned bit)
	{
		return (((reg & (1u << bit)) != 0u));
	}

	static constexpr unsigned GetRegisterValue(unsigned reg, unsigned bitOffset, unsigned bits)
	{
		return ((reg >> bitOffset) & bits);
	}

	void static AddFeature(Vector<String>& features, const String& name, unsigned int reg, unsigned int bit)
	{
		if (FeatureCheck(reg, bit))
		{
			features.push_back(name);
		}
	}

	static void GetAMDSpecifics(
		unsigned int maximumStandardFunction,
		unsigned int maximumExtendedFunction,
		String& modelName,
		Vector<String>& features,
		Map<String, String>& additionalInfo)
	{
		// See https://www.amd.com/system/files/TechDocs/24594.pdf
		unsigned eax, ebx, edx, ecx;
		eax = ebx = ecx = edx = 0;

		// EAX = 0x1 => Family, Model, Stepping Identifiers
		CPUID(0x1, &eax, &ebx, &ecx, &edx);

		// EAX
		additionalInfo.emplace(Stepping, std::to_string(((eax >> 0u) & 0xFu)));
		{
			unsigned int baseModel = GetRegisterValue(eax, 4, 0xF);
			unsigned int baseFamily = GetRegisterValue(eax, 8, 0xF);
			if (baseFamily >= 0xFu)
			{
				additionalInfo.emplace(Family, std::to_string(baseFamily + GetRegisterValue(eax, 20, 0xFF)));
				additionalInfo.emplace(Model, std::to_string(GetRegisterValue(eax, 19, 0xF) << 4u | baseModel));
			}
			else
			{
				additionalInfo.emplace(Family, std::to_string(baseFamily));
				additionalInfo.emplace(Model, std::to_string(baseModel));
			}
		}

		// ECX
		AddFeature(features, "RDRAND", ecx, 30);
		AddFeature(features, "F16C", ecx, 29);
		AddFeature(features, "AVX", ecx, 28);
		AddFeature(features, "AES", ecx, 25);
		AddFeature(features, "SSE42", ecx, 20);
		AddFeature(features, "SSE41", ecx, 19);
		AddFeature(features, "FMA", ecx, 12);
		AddFeature(features, "SSSE3", ecx, 9);
		AddFeature(features, "SSE3", ecx, 0);

		// EDX
		AddFeature(features, "SSE2", edx, 26);
		AddFeature(features, "SSE", edx, 25);
		AddFeature(features, "CMOV", edx, 15);

		// EAX = 0x7 => Structured Extended Feature Identifiers
		eax = ebx = ecx = edx = 0;
		if (maximumStandardFunction >= 0x7)
		{
			ecx = 0;
			CPUID(0x7, &eax, &ebx, &ecx, &edx);
		}

		// EBX
		AddFeature(features, "SHA", ebx, 29);
		AddFeature(features, "BMI2", ebx, 8);
		AddFeature(features, "AVX2", ebx, 5);
		AddFeature(features, "BMI1", ebx, 3);
		AddFeature(features, "RDSEED", ebx, 3);

		// AMD now supports AVX512, see https://www.amd.com/system/files/TechDocs/56713-B1-PUB_3.04.zip
		AddFeature(features, "AVX512-VL", ebx, 31);
		AddFeature(features, "AVX512-BW", ebx, 30);
		AddFeature(features, "AVX512-CD", ebx, 28);
		AddFeature(features, "AVX512-IFMA", ebx, 21);
		AddFeature(features, "AVX512-DQ", ebx, 17);
		AddFeature(features, "AVX512-F", ebx, 16);

		// ECX
		AddFeature(features, "AVX512-VPOPCNTDQ", ecx, 14);
		AddFeature(features, "AVX512-BITALG", ecx, 12);
		AddFeature(features, "AVX512-VNNI", ecx, 11);
		AddFeature(features, "VAES", ecx, 9);
		AddFeature(features, "AVX512-VBMI2", ecx, 6);
		AddFeature(features, "AVX512-VBMI", ecx, 1);

		// EAX = 0x1 => More features
		eax = ebx = ecx = edx = 0;
		if (maximumStandardFunction >= 0x7)
		{
			eax = 0x1;
			CPUID(0x7, &eax, &ebx, &ecx, &edx);
		}

		AddFeature(features, "AVX512-BF", eax, 5);

		// EAX = 0x80000002 - 0x80000004 => Processor Name String Identifier
		for (int i = 2; i <= 4; ++i)
		{
			eax = ebx = ecx = edx = 0;
			CPUID(0x80000000 + i, &eax, &ebx, &ecx, &edx);
			modelName.append((char*)(&eax), 4);
			modelName.append((char*)(&ebx), 4);
			modelName.append((char*)(&ecx), 4);
			modelName.append((char*)(&edx), 4);
		}

		// EAX = 0x80000001 -> Extended Processor and Processor Feature Identifiers
		eax = ebx = ecx = edx = 0;
		CPUID(0x80000001, &eax, &ebx, &ecx, &edx);

		// ECX
		AddFeature(features, "FMA4", ecx, 16);
		AddFeature(features, "XOP", ecx, 11);
		AddFeature(features, "3DNowPrefetch", ecx, 8);
		AddFeature(features, "SSE4A", ecx, 6);
		AddFeature(features, "ABM", ecx, 5);
		AddFeature(features, "SVM", ecx, 2);

		// EDX
		AddFeature(features, "3DNow", edx, 31);
		AddFeature(features, "3DNowExt", edx, 30);
		AddFeature(features, "MMX", edx, 23);
		AddFeature(features, "MMXExt", edx, 22);
	}

	static void GetIntelSpecifics(
		unsigned int maximumStandardFunction,
		unsigned int maximumExtendedFunction,
		String& modelName,
		Vector<String>& features,
		Map<String, String>& additionalInfo)
	{
		unsigned eax, ebx, edx, ecx;

		eax = ebx = ecx = edx = 0;
		CPUID(0x1, &eax, &ebx, &ecx, &edx);

		additionalInfo.emplace(Stepping, std::to_string(((eax >> 0u) & 0xFu)));

		{
			unsigned int baseModel = GetRegisterValue(eax, 4, 0xF);
			unsigned int baseFamily = GetRegisterValue(eax, 8, 0xF);
			if (baseFamily == 0xFu)
			{
				additionalInfo.emplace(Family, std::to_string(baseFamily + GetRegisterValue(eax, 20, 0xFF)));
			}
			else
			{
				additionalInfo.emplace(Family, std::to_string(baseFamily));
			}

			if (baseFamily == 0xFu || baseFamily == 0x6u)
			{
				additionalInfo.emplace(Model, std::to_string((baseModel << 4u) + GetRegisterValue(eax, 19, 0xF)));
			}
			else
			{
				additionalInfo.emplace(Model, std::to_string(baseModel));
			}
		}

		AddFeature(features, "F16C", ecx, 29);
		AddFeature(features, "RDRAND", ecx, 30);
		AddFeature(features, "CMOV", edx, 15);
		AddFeature(features, "MMX", edx, 23);
		AddFeature(features, "MMXExt", 0, 0);    // Not supported by Intel
		AddFeature(features, "SSE", edx, 25);
		AddFeature(features, "SSE2", edx, 26);
		AddFeature(features, "SSE3", ecx, 0);
		AddFeature(features, "SSSE3", ecx, 9);
		AddFeature(features, "SSE41", ecx, 19);
		AddFeature(features, "SSE42", ecx, 20);
		AddFeature(features, "SSE4A", 0, 0);    // Not supported by Intel
		AddFeature(features, "FMA", ecx, 12);
		AddFeature(features, "FMA4", 0, 0);    // Not supported by Intel
		AddFeature(features, "AES", ecx, 25);
		AddFeature(features, "AVX", ecx, 28);


		eax = ebx = ecx = edx = 0;
		if (maximumStandardFunction >= 7)
		{
			CPUID(0x7, &eax, &ebx, &ecx, &edx);
		}

		// https://software.intel.com/sites/default/files/managed/c5/15/architecture-instruction-set-extensions-programming-reference.pdf
		AddFeature(features, "AVX2", ebx, 5);
		AddFeature(features, "AVX512-F", ebx, 16);
		AddFeature(features, "AVX512-BW", ebx, 30);
		AddFeature(features, "AVX512-VL", ebx, 31);
		AddFeature(features, "AVX512-DQ", ebx, 17);
		AddFeature(features, "AVX512-IFMA", ebx, 21);
		AddFeature(features, "AVX512-PF", ebx, 26);
		AddFeature(features, "AVX512-ER", ebx, 27);
		AddFeature(features, "AVX512-CD", ebx, 28);
		AddFeature(features, "AVX512-VBMI", ecx, 1);
		AddFeature(features, "AVX512-VBMI2", ecx, 6);
		AddFeature(features, "AVX512-VNNI", ecx, 11);
		AddFeature(features, "AVX512-4VNNIW", edx, 2);
		AddFeature(features, "AVX512-BITALG", ecx, 12);
		AddFeature(features, "AVX512-VPOPCNTDQ", ecx, 14);
		AddFeature(features, "AVX512-VPCLMULQDQ", ecx, 10);
		AddFeature(features, "AVX512-4FMAPS", edx, 3);
		AddFeature(features, "AVX512-GFNI", ecx, 8);
		AddFeature(features, "AVX512-VAES", ecx, 9);
		AddFeature(features, "AVX512-P2INTERSECT", edx, 8);
		AddFeature(features, "SHA", ebx, 29);
		AddFeature(features, "BMI", ebx, 3);
		AddFeature(features, "BMI2", ebx, 8);

		for (int i = 2; i <= 4; ++i)
		{
			eax = ebx = ecx = edx = 0;
			CPUID(0x80000000 + i, &eax, &ebx, &ecx, &edx);
			modelName.append((char*)(&eax), 4);
			modelName.append((char*)(&ebx), 4);
			modelName.append((char*)(&ecx), 4);
			modelName.append((char*)(&edx), 4);
		}
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

		unsigned int maximumStandardFunction = 0;
		unsigned int maximumExtendedFunction = 0;

		String vendorName;

		maximumStandardFunction = eax;
		vendorName.append((char*)&ebx, 4);
		vendorName.append((char*)&edx, 4);
		vendorName.append((char*)&ecx, 4);

		// EAX = 0x80000000 => return maximum extended function on EAX
		CPUID(0x80000000, &eax, &ebx, &ecx, &edx);
		maximumExtendedFunction = eax;

		String modelName;
		Vector<String> features;
		Map<String, String> additionalInfo;

		if (vendorName == "AuthenticAMD")
		{
			GetAMDSpecifics(maximumStandardFunction, maximumExtendedFunction, modelName, features, additionalInfo);
		}
		else if (vendorName == "GenuineIntel")
		{
			GetIntelSpecifics(maximumStandardFunction, maximumExtendedFunction, modelName, features, additionalInfo);
		}

		return {
			"x86_64",
			vendorName,
			SanitizeModelName(modelName),
			std::move(features),
			std::move(additionalInfo)
		};
	}

} // Elpida

#endif