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
// Created by klapeto on 27/10/20.
//

#if defined(__x86_64__) || defined(_M_X64)

#include "Elpida/SystemInfo/CpuInfo.hpp"

#include "Elpida/Timer.hpp"
#include "Elpida/Utilities/OsUtilities.hpp"

#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <functional>

#ifdef _MSC_VER
#include <intrin.h>
#else

#include <cpuid.h>

#endif

static constexpr bool featureCheck(unsigned reg, unsigned bit)
{
	return (((reg & (1u << bit)) != 0u));
}

static constexpr unsigned getRegisterValue(unsigned reg, unsigned bitOffset, unsigned bits)
{
	return ((reg >> bitOffset) & bits);
}

static inline void cpuid(unsigned func, unsigned* eax, unsigned* ebx, unsigned* ecx, unsigned* edx)
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

static inline unsigned long rdtscp()
{
#ifdef _MSC_VER
	return __rdtsc();
#else
	unsigned long a, d, c;
	asm volatile("rdtsc" : "=a" (a), "=d" (d), "=c" (c));
	return (a | (d << 32u));
#endif
}

namespace Elpida
{

	static const char* stepping = "Stepping";
	static const char* model = "Model";
	static const char* family = "Family";
	static const char* turboBoost = "Turbo Boost";
	static const char* turboBoost3 = "Turbo Boost 3";

	static float getTscFrequency()
	{
		OsUtilities::setCurrentThreadAffinity(0);

		unsigned long startCycles;
		unsigned long endCycles;

		auto start = Timer::now();

		startCycles = rdtscp();

		std::this_thread::sleep_for(std::chrono::microseconds(1));

		endCycles = rdtscp();

		auto end = Timer::now();

		auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

		return (((float)(endCycles - startCycles)) * 1000000000.f) / (float)nanoseconds.count();
	}

	void static
	addFeature(std::vector<CpuFeature>& _features, const std::string& name, unsigned int reg, unsigned int bit)
	{
		static std::unordered_map<std::string, std::string> FeaturesNames =
				{
						{ "CMOV",              "Conditional move instructions" },
						{ "MMX",               "MMX instructions support" },
						{ "MmxExt",            "AMD extensions to MMX instructions" },
						{ "SSE",               "SSE instructions support" },
						{ "SSE2",              "SSE2 instructions support" },
						{ "SSE3",              "SSE3 instructions support" },
						{ "SSSE3",             "Supplemental SSE3 instructions support" },
						{ "SSE41",             "SSE4.1 instructions support" },
						{ "SSE42",             "SSE4.2 instructions support" },
						{ "SSE4A",             "SSE4A instructions support" },
						{ "FMA",               "FMA instructions support" },
						{ "FMA4",              "Four-operand FMA instructions support" },
						{ "AES",               "AES instructions support" },
						{ "AVX",               "AVX instructions support" },
						{ "AVX2",              "AVX2 instructions support" },
						{ "AVX512-F",          "AVX512 Foundation" },
						{ "AVX512-BW",         "AVX-512 Byte and Word instructions support" },
						{ "AVX512-DQ",         "AVX-512 DWORD and QWORD instructions support" },
						{ "AVX512-IFMA",       "AVX-512 Integer FMA instructions support" },
						{ "AVX512-PF",         "AVX-512 Prefetch instructions support" },
						{ "AVX512-ER",         "AVX-512 Exp. and Recp. instructions support" },
						{ "AVX512-CD",         "AVX-512 Conflict Detection instructions support" },
						{ "AVX512-VBMI",       "AVX-512 Vector BMI instructions support" },
						{ "AVX512-VBMI2",      "AVX-512 Vector BMI2 instructions support" },
						{ "AVX512-VNNI",       "AVX-512 Vector Neural Network instructions support" },
						{ "AVX512-4VNNIW",     "AVX-512 4-register Neural Network instructions support" },
						{ "AVX512-BITALG",     "AVX-512 BITALG instructions support" },
						{ "AVX512-VL",         "AVX-512 Vector Length Extensions instructions support" },
						{ "AVX512-VPOPCNTDQ",  "AVX-512 Vector Population Count D/Q instructions support" },
						{ "AVX512-VPCLMULQDQ", "AVX-512 CLMUL instruction set (VEX-256/EVEX) instructions support" },
						{ "AVX512-4FMAPS",     "AVX-512 4-register Multiply Accumulation Single precision instructions support" },
						{ "AVX512-GFNI",       "AVX-512 Galois Field instructions support" },
						{ "AVX512-VAES",       "AVX-512 AES instruction set (VEX-256/EVEX) instructions support" },
						{ "SHA",               "Intel SHA extensions support" },
						{ "XOP",               "Extended operation support" },
						{ "3DNow",             "3DNow! instruction support" },
						{ "3DNowExt",          "AMD extensions to 3DNow! instructions" },
						{ "3DNowPrefetch",     "PREFETCH and PREFETCHW instruction support" },
						{ "BMI1",              "Bit manipulation group 1 instruction support" },
						{ "BMI2",              "Bit manipulation group 2 instruction support" },
						{ "ABM",               "Advanced bit manipulation" },
						{ "F16C",              "Half-precision convert instruction support" },
						{ "RDRAND",            "RDRAND (HW RNG) instruction support" },
						{ "SGX",               "Intel Software Guard Extensions BMI1 instructions support" },
						{ "SVM",               "Secure Virtual Machine" },
						{ "VAES",              "VAES 256-bit instructions" },
						{ "FP256",             "Full Width AVX256 execution" },
						{ "Turbo Boost",       "Cpu Turbo Boost" },
						{ "Turbo Boost 3",     "Cpu Turbo Boost 3" },
				};

		_features.emplace_back(name, FeaturesNames[name], featureCheck(reg, bit));
	}

	static void sanitizeBrand(std::string& _processorBrand)
	{
		size_t index = _processorBrand.size();
		for (auto itr = _processorBrand.rbegin(); itr != _processorBrand.rend(); ++itr)
		{
			if (iscntrl(*itr) || isspace(*itr))
			{
				index--;
				continue;
			}
			break;
		}

		_processorBrand = _processorBrand.substr(0, index);
	}

	static void getAMDFeatures(
			unsigned _maximumStandardFunction,
			unsigned _maximumExtendedFunction,
			std::string& _processorBrand,
			std::vector<CpuCache>& _caches,
			std::unordered_map<std::string, std::string>& _additionalInformation,
			std::vector<CpuFeature>& _features,
			bool& _smt)
	{
		unsigned eax, ebx, edx, ecx;

		//===================================
		// 0x1
		//===================================
		eax = ebx = ecx = edx = 0;
		cpuid(0x1, &eax, &ebx, &ecx, &edx);

		// EAX
		_additionalInformation.insert({ stepping, std::to_string(((eax >> 0u) & 0xFu)) });

		{

			unsigned baseModel = getRegisterValue(eax, 4, 0xF);
			unsigned baseFamily = getRegisterValue(eax, 8, 0xF);
			if (baseFamily >= 0xFu)
			{
				_additionalInformation.insert({ family, std::to_string(baseFamily + getRegisterValue(eax, 20, 0xFF)) });
				_additionalInformation
						.insert({ model, std::to_string(getRegisterValue(eax, 19, 0xF) << 4u | baseModel) });
			}
			{
				_additionalInformation.insert({ family, std::to_string(baseFamily) });
				_additionalInformation.insert({ model, std::to_string(baseModel) });
			}
		}

		// ECX
		addFeature(_features, "RDRAND", ecx, 30);
		addFeature(_features, "F16C", ecx, 29);
		addFeature(_features, "AVX", ecx, 28);
		addFeature(_features, "AES", ecx, 25);
		addFeature(_features, "SSE42", ecx, 20);
		addFeature(_features, "SSE41", ecx, 19);
		addFeature(_features, "FMA", ecx, 12);
		addFeature(_features, "SSSE3", ecx, 9);
		addFeature(_features, "SSE3", ecx, 0);


		// EDX
		_smt = featureCheck(edx, 28);
		addFeature(_features, "SSE2", edx, 26);
		addFeature(_features, "SSE", edx, 25);
		addFeature(_features, "CMOV", edx, 15);



		//===================================
		// 0x7
		//===================================
		eax = ebx = ecx = edx = 0;
		if (_maximumStandardFunction >= 0x7)
		{
			ecx = 0;
			cpuid(0x7, &eax, &ebx, &ecx, &edx);
		}

		// EBX
		addFeature(_features, "SHA", ebx, 29);
		addFeature(_features, "BMI2", ebx, 8);
		addFeature(_features, "AVX2", ebx, 5);
		addFeature(_features, "BMI1", ebx, 3);

		// ECX
		addFeature(_features, "VAES", ecx, 9);



		//===================================
		// Brand
		//===================================
		for (int i = 2; i <= 4; ++i)
		{
			eax = ebx = ecx = edx = 0;
			cpuid(0x80000000 + i, &eax, &ebx, &ecx, &edx);
			_processorBrand.append((char*)(&eax), 4);
			_processorBrand.append((char*)(&ebx), 4);
			_processorBrand.append((char*)(&ecx), 4);
			_processorBrand.append((char*)(&edx), 4);
		}

		//===================================
		// 0x80000001
		//===================================
		eax = ebx = ecx = edx = 0;
		cpuid(0x80000001, &eax, &ebx, &ecx, &edx);

		// ECX
		addFeature(_features, "FMA4", ecx, 16);
		addFeature(_features, "XOP", ecx, 11);
		addFeature(_features, "3DNowPrefetch", ecx, 8);
		addFeature(_features, "SSE4A", ecx, 6);
		addFeature(_features, "ABM", ecx, 5);
		addFeature(_features, "SVM", ecx, 2);

		// EDX
		addFeature(_features, "3DNow", edx, 31);
		addFeature(_features, "3DNowExt", edx, 30);
		//_rdtscp = featureCheck(edx, 27);
		addFeature(_features, "Mmx", edx, 23);
		addFeature(_features, "MmxExt", edx, 22);


		// Not supported by AMD yet
		addFeature(_features, "AVX512-F", 0, 16);
		addFeature(_features, "AVX512-BW", 0, 30);
		addFeature(_features, "AVX512-VL", 0, 31);
		addFeature(_features, "AVX512-DQ", 0, 17);
		addFeature(_features, "AVX512-IFMA", 0, 21);
		addFeature(_features, "AVX512-PF", 0, 26);
		addFeature(_features, "AVX512-ER", 0, 27);
		addFeature(_features, "AVX512-CD", 0, 28);
		addFeature(_features, "AVX512-VBMI", 0, 1);
		addFeature(_features, "AVX512-VBMI2", 0, 6);
		addFeature(_features, "AVX512-VNNI", 0, 11);
		addFeature(_features, "AVX512-4VNNIW", 0, 2);
		addFeature(_features, "AVX512-BITALG", 0, 12);
		addFeature(_features, "AVX512-VPOPCNTDQ", 0, 14);
		addFeature(_features, "AVX512-VPCLMULQDQ", 0, 10);
		addFeature(_features, "AVX512-4FMAPS", 0, 3);
		addFeature(_features, "AVX512-GFNI", 0, 8);
		addFeature(_features, "AVX512-VAES", 0, 9);
		addFeature(_features, "SGX", 0, 2);


		if (_maximumExtendedFunction >= 0x80000005)
		{
			std::string cacheAssociativities[] = {
					"Disabled",
					"Direct mapped",
					"2-Way",
					"",
					"4-Way",
					"",
					"8-Way",
					"",
					"16-Way",
					"",
					"32-Way",
					"48-Way",
					"64-Way",
					"96-Way",
					"128-Way",
					"Fully"
			};


			//===================================
			// 0x80000005
			//===================================
			eax = ebx = ecx = edx = 0;
			cpuid(0x80000005, &eax, &ebx, &ecx, &edx);

			_caches.emplace_back(1, "L1 Instruction Cache",
					std::to_string(getRegisterValue(edx, 16, 0xFF)) + "-way",
					(getRegisterValue(edx, 24, 0xFF) * 1000),
					getRegisterValue(edx, 0, 0xFF));

			_caches.emplace_back(1, "L1 Data Cache",
					std::to_string(getRegisterValue(ecx, 16, 0xFF)) + "-way",
					(getRegisterValue(ecx, 24, 0xFF) * 1000),
					getRegisterValue(ecx, 0, 0xFF));


			//===================================
			// 0x80000006
			//===================================
			eax = ebx = ecx = edx = 0;
			cpuid(0x80000006, &eax, &ebx, &ecx, &edx);

			_caches.emplace_back(2, "L2 Cache",
					cacheAssociativities[getRegisterValue(ecx, 12, 0xF)],
					(getRegisterValue(ecx, 16, 0xFFFF) * 1000),
					getRegisterValue(ecx, 0, 0xFF));

			_caches.emplace_back(3, "L3 Cache",
					cacheAssociativities[getRegisterValue(edx, 12, 0xF)],
					(getRegisterValue(edx, 18, 0x3FFF) * 512000),
					getRegisterValue(edx, 0, 0xFF));
		}

		if (_maximumExtendedFunction >= 0x80000007)
		{
			//===================================
			// 0x80000007
			//===================================
			eax = ebx = ecx = edx = 0;
			cpuid(0x80000007, &eax, &ebx, &ecx, &edx);

			addFeature(_features, "Turbo Boost", edx, 9);
			addFeature(_features, "Turbo Boost 3", 0, 9); // Not Supported
		}

		if (_maximumExtendedFunction >= 0x8000001A)
		{
			//===================================
			// 0x80000007
			//===================================
			eax = ebx = ecx = edx = 0;
			cpuid(0x8000001A, &eax, &ebx, &ecx, &edx);

			addFeature(_features, "FP256", eax, 2);
		}
	}

	static void getIntelFeatures(
			unsigned _maximumStandardFunction,
			unsigned _maximumExtendedFunction,
			std::string& _processorBrand,
			std::vector<CpuCache>& _caches,
			std::unordered_map<std::string, std::string>& _additionalInformation,
			std::vector<CpuFeature>& _features,
			bool& _smt)
	{
		unsigned eax, ebx, edx, ecx;

		eax = ebx = ecx = edx = 0;
		cpuid(0x1, &eax, &ebx, &ecx, &edx);

		_additionalInformation.insert({ stepping, std::to_string(((eax >> 0u) & 0xFu)) });

		{
			unsigned baseModel = getRegisterValue(eax, 4, 0xF);
			unsigned baseFamily = getRegisterValue(eax, 8, 0xF);
			if (baseFamily == 0xFu)
			{
				_additionalInformation.insert({ family, std::to_string(baseFamily + getRegisterValue(eax, 20, 0xFF)) });
			}
			else
			{
				_additionalInformation.insert({ family, std::to_string(baseFamily) });
			}

			if (baseFamily == 0xFu || baseFamily == 0x6u)
			{
				_additionalInformation
						.insert({ model, std::to_string((baseModel << 4u) + getRegisterValue(eax, 19, 0xF)) });
			}
			else
			{
				_additionalInformation.insert({ model, std::to_string(baseModel) });
			}

			_smt = featureCheck(edx, 28);

			//_rdtscp = featureCheck(edx, 4);

			addFeature(_features, "F16C", ecx, 29);
			addFeature(_features, "RDRAND", ecx, 30);
			addFeature(_features, "CMOV", edx, 15);
			addFeature(_features, "MMX", edx, 23);
			addFeature(_features, "MmxExt", 0, 0);    // Not supported by Intel
			addFeature(_features, "SSE", edx, 25);
			addFeature(_features, "SSE2", edx, 26);
			addFeature(_features, "SSE3", ecx, 0);
			addFeature(_features, "SSSE3", ecx, 9);
			addFeature(_features, "SSE41", ecx, 19);
			addFeature(_features, "SSE42", ecx, 20);
			addFeature(_features, "SSE4A", 0, 0);    // Not supported by Intel
			addFeature(_features, "FMA", ecx, 12);
			addFeature(_features, "FMA4", 0, 0);    // Not supported by Intel
			addFeature(_features, "AES", ecx, 25);
			addFeature(_features, "AVX", ecx, 28);

			eax = ebx = ecx = edx = 0;
			if (_maximumStandardFunction >= 7)
			{
				cpuid(0x7, &eax, &ebx, &ecx, &edx);
			}

			// https://software.intel.com/sites/default/files/managed/c5/15/architecture-instruction-set-extensions-programming-reference.pdf
			addFeature(_features, "AVX2", ebx, 5);
			addFeature(_features, "AVX512-F", ebx, 16);
			addFeature(_features, "AVX512-BW", ebx, 30);
			addFeature(_features, "AVX512-VL", ebx, 31);
			addFeature(_features, "AVX512-DQ", ebx, 17);
			addFeature(_features, "AVX512-IFMA", ebx, 21);
			addFeature(_features, "AVX512-PF", ebx, 26);
			addFeature(_features, "AVX512-ER", ebx, 27);
			addFeature(_features, "AVX512-CD", ebx, 28);
			addFeature(_features, "AVX512-VBMI", ecx, 1);
			addFeature(_features, "AVX512-VBMI2", ecx, 6);
			addFeature(_features, "AVX512-VNNI", ecx, 11);
			addFeature(_features, "AVX512-4VNNIW", edx, 2);
			addFeature(_features, "AVX512-BITALG", ecx, 12);
			addFeature(_features, "AVX512-VPOPCNTDQ", ecx, 14);
			addFeature(_features, "AVX512-VPCLMULQDQ", ecx, 10);
			addFeature(_features, "AVX512-4FMAPS", edx, 3);
			addFeature(_features, "AVX512-GFNI", ecx, 8);
			addFeature(_features, "AVX512-VAES", ecx, 9);
			addFeature(_features, "SHA", ebx, 29);
			addFeature(_features, "SGX", ebx, 2);
			addFeature(_features, "BMI", ebx, 3);
			addFeature(_features, "BMI2", ebx, 8);
			//addFeature("FP256", 1, 0);

			addFeature(_features, "VAES", 0, 0);
			addFeature(_features, "XOP", 0, 0);
			addFeature(_features, "3DNow", 0, 0);
			addFeature(_features, "3DNowExt", 0, 0);
			addFeature(_features, "3DNowPrefetch", 0, 0);
			addFeature(_features, "ABM", 0, 0);
			addFeature(_features, "SVM", 0, 0);

			for (int i = 2; i <= 4; ++i)
			{
				eax = ebx = ecx = edx = 0;
				cpuid(0x80000000 + i, &eax, &ebx, &ecx, &edx);
				_processorBrand.append((char*)(&eax), 4);
				_processorBrand.append((char*)(&ebx), 4);
				_processorBrand.append((char*)(&ecx), 4);
				_processorBrand.append((char*)(&edx), 4);
			}

			if (_maximumStandardFunction >= 0x6)
			{
				cpuid(0x6, &eax, &ebx, &ecx, &edx);
				addFeature(_features, "Turbo Boost", eax, 1);
				addFeature(_features, "Turbo Boost 3", eax, 14);
			}

			if (_maximumStandardFunction >= 0x4)
			{

				int i = 0;

				std::string
						intelCacheTypes[] = { "No Cache", "Data Cache", "Instruction Cache", "Unified Cache" };

				while (true)
				{
					eax = ebx = edx = 0;
					ecx = i;
					cpuid(0x4, &eax, &ebx, &ecx, &edx);
					if (getRegisterValue(eax, 0, 0x1F) == 0) break;

					auto level = getRegisterValue(eax, 5, 0x7);

					auto name = "L" + std::to_string(level) + " "
								+ intelCacheTypes[getRegisterValue(eax, 0, 0x1F) < 4 ? getRegisterValue(eax, 0, 0x1F)
																					 : 0];

					auto size = (getRegisterValue(ebx, 22, 0x3FF) + 1) * (getRegisterValue(ebx, 12, 0x3FF) + 1)
								* (getRegisterValue(ebx, 0, 0xFFF) + 1) * (getRegisterValue(ecx, 0, 0x7FFFFFFF) + 1);

					auto associativity = std::to_string((getRegisterValue(ebx, 22, 0x3FF) + 1)) + "-way";
					auto lineSize = (getRegisterValue(ebx, 0, 0xFFF) + 1);

					_caches.emplace_back(level, name, associativity, size, lineSize);

					i++;
				}
			}
		}
	}

	CpuInfo::CpuInfo()
	{
		_architecture = "x86-64";

		unsigned eax = 0, ebx = 0, ecx = 0, edx = 0;

		cpuid(0x0, &eax, &ebx, &ecx, &edx);

		unsigned _maximumStandardFunction = 0;
		unsigned _maximumExtendedFunction = 0;

		if (ebx != 0)
		{
			_maximumStandardFunction = eax;
			_vendorName.append((char*)&ebx, 4);
			_vendorName.append((char*)&edx, 4);
			_vendorName.append((char*)&ecx, 4);
			cpuid(0x80000000, &eax, &ebx, &ecx, &edx);
			_maximumExtendedFunction = eax;
		}
		else
		{
			cpuid(0x80000000, &eax, &ebx, &ecx, &edx);
			_maximumExtendedFunction = eax;
			_vendorName.append((char*)&ebx, 4);
			_vendorName.append((char*)&edx, 4);
			_vendorName.append((char*)&ecx, 4);
		}

		if (_vendorName == "AuthenticAMD")
		{
			getAMDFeatures(_maximumStandardFunction,
					_maximumExtendedFunction,
					_modelName,
					_caches,
					_additionalInformation,
					_features,
					_smt);
		}
		else if (_vendorName == "GenuineIntel")
		{
			getIntelFeatures(_maximumStandardFunction,
					_maximumExtendedFunction,
					_modelName,
					_caches,
					_additionalInformation,
					_features,
					_smt);
		}

		_frequency = getTscFrequency();

		sanitizeBrand(_modelName);
	}
}

#endif