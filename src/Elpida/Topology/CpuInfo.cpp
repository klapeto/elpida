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

/*
 * Cpu.cpp
 *
 *  Created on: 10 Μαρ 2018
 *      Author: klapeto
 */

#include "Elpida/Topology/CpuInfo.hpp"

#ifdef _MSC_VER
#include <intrin.h>
#else
#include <cpuid.h>
#endif

#include <chrono>
#include <utility>
#include <cstddef>

#include "Elpida/Engine/Task/TaskThread.hpp"
#include "Elpida/Timer.hpp"

#define columnWidth 20
#define out(prefix, value)    output << std::left  <<std::setw(columnWidth) << prefix  << std::setw(columnWidth) << value << _newLine
#define featureCheck(reg, bit) (((reg & (1 << bit)) != 0))
#define getRegisterValue(reg, bitOffset, bits) ((reg >> bitOffset) & bits)
#define addFeature(category, name, reg, bit) category.push_back( { name, FeaturesNames[name], featureCheck(reg, bit) })

#define vendorByteSize 12

static void cpuid(unsigned func, unsigned* eax, unsigned* ebx, unsigned* ecx, unsigned* edx) {
#ifdef _MSC_VER
	int regs[4] = { -1 };
	__cpuidex(regs, func, *exc);
	*eax = regs[0];
	*ebx = regs[1];
	*ecx = regs[2];
	*edx = regs[3];
#else
	__get_cpuid_count(func, *ecx, eax, ebx, ecx, edx);
#endif
}

namespace Elpida
{

	std::string amdCacheAssociativities[] = {
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
		"Fully" };

	std::string intelCacheTypes[] = { "No Cache", "Data Cache", "Instruction Cache", "Unified Cache" };

	CpuInfo::CpuInfo()
		:
		_vendor(Vendor::Unknown),
		_model(-1),
		_family(-1),
		_cacheLineSize(-1),
		_maximumStandardFunction(0),
		_maximumExtendedFunction(0),
		_tscFrequency(0),
		_tscTimeRatio(1.0),
		_hyperThreading(false),
		_rdtscp(false)
	{
		unsigned eax = 0, ebx = 0, ecx = 0, edx = 0;

		cpuid(0x0, &eax, &ebx, &ecx, &edx);

		if (ebx != 0)
		{
			_maximumStandardFunction = eax;
			_vendorString.append((char*)&ebx, 4);
			_vendorString.append((char*)&edx, 4);
			_vendorString.append((char*)&ecx, 4);
			cpuid(0x80000000, &eax, &ebx, &ecx, &edx);
			_maximumExtendedFunction = eax;
		}
		else
		{
			cpuid(0x80000000, &eax, &ebx, &ecx, &edx);
			_maximumExtendedFunction = eax;
			_vendorString.append((char*)&ebx, 4);
			_vendorString.append((char*)&edx, 4);
			_vendorString.append((char*)&ecx, 4);
		}

		if (_vendorString == "AuthenticAMD")
		{
			_vendor = Vendor::AMD;
		}
		else if (_vendorString == "GenuineIntel")
		{
			_vendor = Vendor::Intel;
		}

		switch (_vendor)
		{
		case Vendor::AMD:
			getAMDFeatures();
			break;
		case Vendor::Intel:
			getIntelFeatures();
			break;
		default:
			break;
		}

		if (_rdtscp)
		{
			getTscFrequency();
		}
	}

	CpuInfo::~CpuInfo()
	{

	}

	static inline unsigned long rdtscp()
	{
#ifdef _MSC_VER
			return __rdtsc();
#else
		unsigned long a, d, c;
		asm volatile("rdtscp" : "=a" (a), "=d" (d), "=c" (c));
		return (a | (d << 32));
#endif
	}

	void CpuInfo::getTscFrequency()
	{
		size_t nowOverhead = Timer::getNowOverhead();
		TaskThread::setCurrentThreadAffinity(0);

		unsigned long startCycles = 0;
		unsigned long endCycles = 0;

		auto start = Timer::now();

		startCycles = rdtscp();
		std::this_thread::sleep_for(std::chrono::microseconds(1000));

		endCycles = rdtscp();

		auto end = Timer::now();

		auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() - nowOverhead;

		_tscFrequency = (((float)(endCycles - startCycles)) * 1000000000.f) / (float)nanoseconds;
		_tscTimeRatio = (float)nanoseconds / ((float)(endCycles - startCycles));

	}

	void CpuInfo::getAMDFeatures()
	{
		unsigned eax, ebx, edx, ecx;

		cpuid(0x80000001, &eax, &ebx, &ecx, &edx);

		_stepping = ((eax >> 0) & 0xF);

		{
			unsigned model = (eax >> 4) & 0xF;
			unsigned family = ((eax >> 8) & 0xF);
			if (family == 15)
			{
				_family = family + ((eax >> 20) & 0xFF);
				_model = model + (((eax >> 16) & 0x7) << 4);
			}
			else if (family == 6)
			{
				_family = family;
				_model = model + (((eax >> 16) & 0x7) << 4);
			}
			else
			{
				_family = family;
				_model = model;
			}

		}

		cpuid(0x0000001, &eax, &ebx, &ecx, &edx);

		if (featureCheck(edx, 19))
		{
			_cacheLineSize = getRegisterValue(ebx, 8, 0xFF);
		}

		if (featureCheck(edx, 28))
		{
			_hyperThreading = true;
		}

		for (int i = 2; i <= 4; ++i)
		{
			cpuid(0x80000000 + i, &eax, &ebx, &ecx, &edx);
			_processorBrand.append((char*)(&eax), 4);
			_processorBrand.append((char*)(&ebx), 4);
			_processorBrand.append((char*)(&ecx), 4);
			_processorBrand.append((char*)(&edx), 4);
		}

		cpuid(0x80000001, &eax, &ebx, &ecx, &edx);
		addFeature(_instructionExtensions, "CMOV", edx, 15);
		addFeature(_instructionExtensions, "MMX", edx, 23);
		addFeature(_instructionExtensions, "MmxExt", edx, 22);

		cpuid(0x1, &eax, &ebx, &ecx, &edx);
		_rdtscp = featureCheck(edx, 4);
		addFeature(_instructionExtensions, "SSE", edx, 25);
		addFeature(_instructionExtensions, "SSE2", edx, 26);
		addFeature(_instructionExtensions, "SSE3", ecx, 0);
		addFeature(_instructionExtensions, "SSSE3", ecx, 9);
		addFeature(_instructionExtensions, "SSE41", ecx, 19);
		addFeature(_instructionExtensions, "SSE42", ecx, 20);

		cpuid(0x80000001, &eax, &ebx, &ecx, &edx);
		addFeature(_instructionExtensions, "SSE4A", ecx, 6);

		cpuid(0x1, &eax, &ebx, &ecx, &edx);
		addFeature(_instructionExtensions, "FMA", ecx, 12);

		cpuid(0x80000001, &eax, &ebx, &ecx, &edx);
		addFeature(_instructionExtensions, "FMA4", ecx, 16);

		cpuid(0x1, &eax, &ebx, &ecx, &edx);
		addFeature(_instructionExtensions, "AES", ecx, 25);
		addFeature(_instructionExtensions, "AVX", ecx, 28);

		if (_maximumStandardFunction >= 0x7)
		{
			cpuid(0x7, &eax, &ebx, &ecx, &edx);
		}
		else
		{
			eax = ebx = ecx = edx = 0;
		}
		addFeature(_instructionExtensions, "AVX2", ebx, 5);
		// Not supported by AMD
		addFeature(_instructionExtensions, "AVX512-F", 0, 16);
		addFeature(_instructionExtensions, "AVX512-BW", 0, 30);
		addFeature(_instructionExtensions, "AVX512-VL", 0, 31);
		addFeature(_instructionExtensions, "AVX512-DQ", 0, 17);
		addFeature(_instructionExtensions, "AVX512-IFMA", 0, 21);
		addFeature(_instructionExtensions, "AVX512-PF", 0, 26);
		addFeature(_instructionExtensions, "AVX512-ER", 0, 27);
		addFeature(_instructionExtensions, "AVX512-CD", 0, 28);
		addFeature(_instructionExtensions, "AVX512-VBMI", 0, 1);
		addFeature(_instructionExtensions, "AVX512-VBMI2", 0, 6);
		addFeature(_instructionExtensions, "AVX512-VNNI", 0, 11);
		addFeature(_instructionExtensions, "AVX512-4VNNIW", 0, 2);
		addFeature(_instructionExtensions, "AVX512-BITALG", 0, 12);
		addFeature(_instructionExtensions, "AVX512-VPOPCNTDQ", 0, 14);
		addFeature(_instructionExtensions, "AVX512-VPCLMULQDQ", 0, 10);
		addFeature(_instructionExtensions, "AVX512-4FMAPS", 0, 3);
		addFeature(_instructionExtensions, "AVX512-GFNI", 0, 8);
		addFeature(_instructionExtensions, "AVX512-VAES", 0, 9);
		addFeature(_instructionExtensions, "SHA", ebx, 29);
		addFeature(_instructionExtensions, "SGX", 0, 2);

		cpuid(0x80000001, &eax, &ebx, &ecx, &edx);
		addFeature(_instructionExtensions, "XOP", ecx, 11);
		addFeature(_instructionExtensions, "3DNow", edx, 31);
		addFeature(_instructionExtensions, "3DNowExt", edx, 30);
		addFeature(_instructionExtensions, "3DNowPrefetch", ecx, 8);

		if (_maximumStandardFunction >= 0x7)
		{
			cpuid(0x7, &eax, &ebx, &ecx, &edx);
		}
		else
		{
			eax = ebx = ecx = edx = 0;
		}

		addFeature(_instructionExtensions, "BMI1", ebx, 3);
		addFeature(_instructionExtensions, "BMI2", ebx, 8);

		cpuid(0x80000001, &eax, &ebx, &ecx, &edx);
		addFeature(_instructionExtensions, "ABM", ecx, 5);

		cpuid(0x1, &eax, &ebx, &ecx, &edx);
		addFeature(_instructionExtensions, "F16C", ecx, 29);
		addFeature(_instructionExtensions, "RDRAND", ecx, 30);

		if (_maximumExtendedFunction >= 0x80000005)
		{

			cpuid(0x80000005, &eax, &ebx, &ecx, &edx);
			Cache tmpCache;

			tmpCache.name = "L1 Instruction Cache (Per Core)";
			tmpCache.size = (getRegisterValue(edx, 24, 0xFF) * 1000);
			tmpCache.associativity = std::to_string(getRegisterValue(edx, 16, 0xFF)) + "-way";
			tmpCache.linesPerTag = getRegisterValue(edx, 8, 0xFF);
			tmpCache.lineSize = getRegisterValue(edx, 0, 0xFF);
			_caches.push_back(tmpCache);

			tmpCache.name = "L1 Data Cache (Per Core)";
			tmpCache.size = (getRegisterValue(ecx, 24, 0xFF) * 1000);
			tmpCache.associativity = std::to_string(getRegisterValue(ecx, 16, 0xFF)) + "-way";
			tmpCache.linesPerTag = getRegisterValue(ecx, 8, 0xFF);
			tmpCache.lineSize = getRegisterValue(ecx, 0, 0xFF);
			_caches.push_back(tmpCache);

			cpuid(0x80000006, &eax, &ebx, &ecx, &edx);

			tmpCache.name = "L2 Cache (Per Core)";
			tmpCache.size = (getRegisterValue(ecx, 16, 0xFFFF) * 1000);
			tmpCache.associativity = amdCacheAssociativities[getRegisterValue(ecx, 12, 0xF)];
			tmpCache.linesPerTag = getRegisterValue(ecx, 8, 0xF);
			tmpCache.lineSize = getRegisterValue(ecx, 0, 0xFF);
			_caches.push_back(tmpCache);

			tmpCache.name = "L3 Cache (Shared)";
			tmpCache.size = (getRegisterValue(edx, 18, 0x3FFF) * 512000);
			tmpCache.associativity = amdCacheAssociativities[getRegisterValue(edx, 12, 0xF)];
			tmpCache.linesPerTag = getRegisterValue(edx, 8, 0xF);
			tmpCache.lineSize = getRegisterValue(edx, 0, 0xFF);
			_caches.push_back(tmpCache);
		}

		if (_maximumExtendedFunction >= 0x80000007)
		{
			cpuid(0x80000007, &eax, &ebx, &ecx, &edx);
			_turboBoost = featureCheck(edx, 9);
			_invariantTsc = featureCheck(edx, 8);
		}
	}

	std::unordered_map<std::string, CpuFeature> CpuInfo::getInstructionSetSupport() const
	{
		std::unordered_map<std::string, CpuFeature> returnMap;

		for (auto& feature : _instructionExtensions)
		{
			returnMap.emplace(feature.getName(), CpuFeature(feature));
		}
		return returnMap;
	}

	void CpuInfo::getIntelFeatures()
	{
		unsigned eax, ebx, edx, ecx;

		cpuid(0x1, &eax, &ebx, &ecx, &edx);

		_stepping = ((eax >> 0) & 0xF);
		{
			unsigned model = (eax >> 4) & 0xF;
			unsigned family = ((eax >> 8) & 0xF);
			if (family == 0xF)
			{
				_family = family + ((eax >> 20) & 0xFF);
				_model = model + (((eax >> 16) & 0x7) << 4);
			}
			else if (family == 0x6 || family == 0xF)
			{
				_family = family;
				_model = model + (((eax >> 16) & 0x7) << 4);
			}
			else
			{
				_family = family;
				_model = model;
			}

		}

		if (featureCheck(edx, 19))
		{
			_cacheLineSize = getRegisterValue(ebx, 8, 0xFF);
		}

		if (featureCheck(edx, 28))
		{
			_hyperThreading = true;
		}

		_rdtscp = featureCheck(edx, 4);

		addFeature(_instructionExtensions, "CMOV", edx, 15);
		addFeature(_instructionExtensions, "MMX", edx, 23);
		addFeature(_instructionExtensions, "MmxExt", 0, 0);    // Not supported by Intel
		addFeature(_instructionExtensions, "SSE", edx, 25);
		addFeature(_instructionExtensions, "SSE2", edx, 26);
		addFeature(_instructionExtensions, "SSE3", ecx, 0);
		addFeature(_instructionExtensions, "SSSE3", ecx, 9);
		addFeature(_instructionExtensions, "SSE41", ecx, 19);
		addFeature(_instructionExtensions, "SSE42", ecx, 20);
		addFeature(_instructionExtensions, "SSE4A", 0, 0);    // Not supported by Intel
		addFeature(_instructionExtensions, "FMA", ecx, 12);
		addFeature(_instructionExtensions, "FMA4", 0, 0);    // Not supported by Intel
		addFeature(_instructionExtensions, "AES", ecx, 25);
		addFeature(_instructionExtensions, "AVX", ecx, 28);

		if (_maximumStandardFunction >= 7)
		{
			cpuid(0x7, &eax, &ebx, &ecx, &edx);
		}
		else
		{
			eax = ebx = ecx = edx = 0;
		}

		// https://software.intel.com/sites/default/files/managed/c5/15/architecture-instruction-set-extensions-programming-reference.pdf
		addFeature(_instructionExtensions, "AVX2", ebx, 5);
		addFeature(_instructionExtensions, "AVX512-F", ebx, 16);
		addFeature(_instructionExtensions, "AVX512-BW", ebx, 30);
		addFeature(_instructionExtensions, "AVX512-VL", ebx, 31);
		addFeature(_instructionExtensions, "AVX512-DQ", ebx, 17);
		addFeature(_instructionExtensions, "AVX512-IFMA", ebx, 21);
		addFeature(_instructionExtensions, "AVX512-PF", ebx, 26);
		addFeature(_instructionExtensions, "AVX512-ER", ebx, 27);
		addFeature(_instructionExtensions, "AVX512-CD", ebx, 28);
		addFeature(_instructionExtensions, "AVX512-VBMI", ecx, 1);
		addFeature(_instructionExtensions, "AVX512-VBMI2", ecx, 6);
		addFeature(_instructionExtensions, "AVX512-VNNI", ecx, 11);
		addFeature(_instructionExtensions, "AVX512-4VNNIW", edx, 2);
		addFeature(_instructionExtensions, "AVX512-BITALG", ecx, 12);
		addFeature(_instructionExtensions, "AVX512-VPOPCNTDQ", ecx, 14);
		addFeature(_instructionExtensions, "AVX512-VPCLMULQDQ", ecx, 10);
		addFeature(_instructionExtensions, "AVX512-4FMAPS", edx, 3);
		addFeature(_instructionExtensions, "AVX512-GFNI", ecx, 8);
		addFeature(_instructionExtensions, "AVX512-VAES", ecx, 9);
		addFeature(_instructionExtensions, "SHA", ebx, 29);
		addFeature(_instructionExtensions, "SGX", ebx, 2);

		addFeature(_instructionExtensions, "XOP", 0, 0);
		addFeature(_instructionExtensions, "3DNow", 0, 0);
		addFeature(_instructionExtensions, "3DNowExt", 0, 0);
		addFeature(_instructionExtensions, "3DNowPrefetch", 0, 0);
		addFeature(_instructionExtensions, "BMI", ebx, 3);
		addFeature(_instructionExtensions, "BMI2", ebx, 8);
		addFeature(_instructionExtensions, "ABM", 0, 0);

		cpuid(0x1, &eax, &ebx, &ecx, &edx);
		addFeature(_instructionExtensions, "F16C", ecx, 29);
		addFeature(_instructionExtensions, "RDRAND", ecx, 30);

		for (int i = 2; i <= 4; ++i)
		{
			cpuid(0x80000000 + i, &eax, &ebx, &ecx, &edx);
			_processorBrand.append((char*)(&eax), 4);
			_processorBrand.append((char*)(&ebx), 4);
			_processorBrand.append((char*)(&ecx), 4);
			_processorBrand.append((char*)(&edx), 4);
		}

		if (_maximumStandardFunction >= 0x6)
		{
			cpuid(0x6, &eax, &ebx, &ecx, &edx);
			_turboBoost = featureCheck(eax, 1);
			_turboBoost3 = featureCheck(eax, 14);
		}

		if (_maximumStandardFunction >= 0x4)
		{

			int i = 0;

			while (true)
			{
				eax = ebx = edx = 0;
				ecx = i;
				cpuid(0x4, &eax, &ebx, &ecx, &edx);
				if (getRegisterValue(eax, 0, 0x1F) == 0) break;

				Cache tmpCache;

				tmpCache.name = "L" + std::to_string(getRegisterValue(eax, 5, 0x7)) + " "
					+ intelCacheTypes[getRegisterValue(eax, 0, 0x1F) < 4 ? getRegisterValue(eax, 0, 0x1F) : 0];

				tmpCache.size = (getRegisterValue(ebx, 22, 0x3FF) + 1) * (getRegisterValue(ebx, 12, 0x3FF) + 1)
					* (getRegisterValue(ebx, 0, 0xFFF) + 1) * (getRegisterValue(ecx, 0, 0x7FFFFFFF) + 1);

				tmpCache.associativity = std::to_string((getRegisterValue(ebx, 22, 0x3FF) + 1)) + "-way";
				tmpCache.linesPerTag = (getRegisterValue(ebx, 12, 0x3FF) + 1);
				tmpCache.lineSize = (getRegisterValue(ebx, 0, 0xFFF) + 1);
				_caches.push_back(std::move(tmpCache));
				i++;
			}
		}
		if (_maximumExtendedFunction >= 0x80000007)
		{
			cpuid(0x80000007, &eax, &ebx, &ecx, &edx);
			_invariantTsc = featureCheck(edx, 8);
		}

	}

} /* namespace Elpida */

