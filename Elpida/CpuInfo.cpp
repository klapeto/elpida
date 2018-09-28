/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2018  Ioannis Panagiotopoulos
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

#include "Elpida/CpuInfo.hpp"

#include <cpuid.h>
#include <unistd.h>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <unordered_map>
#include <thread>

#include "Elpida/Timer.hpp"
#include "Elpida/TaskThread.hpp"
#include "Elpida/Utilities/TextTable.hpp"
#include "Elpida/Utilities/TextColumn.hpp"

#define columnWidth 20
#define out(prefix, value)	output << std::left  <<std::setw(columnWidth) << prefix  << std::setw(columnWidth) << value << _newLine
#define featureCheck(reg, bit) (((reg & (1 << bit)) != 0))
#define getRegisterValue(reg, bitOffset, bits) ((reg >> bitOffset) & bits)
#define addFeature(category, name, reg, bit) category.push_back( { name, FeaturesNames[name], featureCheck(reg, bit) })

#define vendorByteSize 12

namespace Elpida
{

	String amdCacheAssociativities[] =
		{
		        "Disabled", "Direct mapped", "2-Way", "", "4-Way", "", "8-Way", "", "16-Way", "", "32-Way", "48-Way", "64-Way", "96-Way",
		        "128-Way", "Fully" };

	String intelCacheTypes[] =
		{ "No Cache", "Data Cache", "Instruction Cache", "Unified Cache" };

	CpuInfo::CpuInfo()
			:
			  _vendor(Vendor::Unknown),
			  _model(-1),
			  _family(-1),
			  _cacheLineSize(-1),
			  _physicalCores(1),
			  _logicalProcessors(1),
			  _maximumStandardFunction(0),
			  _maximumExtendedFunction(0),
			  _tscFrequency(0),
			  _tscTimeRatio(1.0),
			  _hyperThreading(false),
			  _rdtscp(false)
	{

		setPadding(4);

		if (__get_cpuid_max(0, nullptr) == 0)
		{
			return;
		}

		unsigned eax = 0, ebx = 0, ecx = 0, edx = 0;

		__get_cpuid(0x0, &eax, &ebx, &ecx, &edx);

		if (ebx != 0)
		{
			_maximumStandardFunction = eax;
			_vendorString.append((char*) &ebx, 4);
			_vendorString.append((char*) &edx, 4);
			_vendorString.append((char*) &ecx, 4);
			__get_cpuid(0x80000000, &eax, &ebx, &ecx, &edx);
			_maximumExtendedFunction = eax;
		}
		else
		{
			__get_cpuid(0x80000000, &eax, &ebx, &ecx, &edx);
			_maximumExtendedFunction = eax;
			_vendorString.append((char*) &ebx, 4);
			_vendorString.append((char*) &edx, 4);
			_vendorString.append((char*) &ecx, 4);
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

	void CpuInfo::exportTo(std::ostream& output) const
	{
		exportBasicInfo(output);
		exportInstructionSetSupportInfo(output);
		exportCacheInfo(output);
	}

	void CpuInfo::exportBasicInfo(std::ostream& output) const
	{
		output << _newLine;
		output << "======================================" << _newLine;
		output << "CPU Info" << _newLine;
		output << "======================================" << _newLine;
		out("Vendor:", _vendorString);
		out("Model:", _processorBrand);
		out("TSC Frequency:", std::to_string((size_t )_tscFrequency / 1000000) + " MHz");
		out("ModelID:", _model);
		out("Family:", _family);
		out("Stepping:", _stepping);
		out("Hyper Threading:", (_hyperThreading ? "true" : "false"));
		out("Turbo Boost:", (_turboBoost ? "true" : "false"));
		out("Turbo Boost 3:", (_turboBoost3 ? "true" : "false"));
		out("Logical Cores:", _logicalProcessors);
	}

	void CpuInfo::exportInstructionSetSupportInfo(std::ostream& output) const
	{
		output << _newLine << _newLine << "CPU Instructions Extensions:" << _newLine;

		TextTable outputTable =
			{ TextColumn("Description", 50), TextColumn("Short", 15), TextColumn("Supported", 10) };

		outputTable.setPadding(4);
		for (auto instruction : _instructionExtensions)
		{
			outputTable.addRow(
				{ instruction.getDescription(), instruction.getName(), (instruction.isSupported() ? "true" : "false") });
		}
		outputTable.exportTo(output);
	}

	void CpuInfo::exportCacheInfo(std::ostream& output) const
	{
		output << _newLine << _newLine << "CPU Caches:" << _newLine << _newLine;

		for (auto cache : _caches)
		{
			out(cache.name, "");
			out("-------------------------", "");
			out("Size: ", std::to_string(cache.size / 1000) + " KB");
			out("Associativity: ", cache.associativity);
			out("Lines per tag: ", cache.linesPerTag);
			out("Line size: ", std::to_string(cache.lineSize) + " Bytes");
			output << _newLine;
		}
	}

	void CpuInfo::getTscFrequency()
	{

		unsigned cycles_high1, cycles_high2, cycles_low1, cycles_low2;

		size_t nowOverhead = Timer::getNowOverhead();
		TaskThread::setCurrentThreadAffinity(0);

		auto start = Timer::now();
		asm volatile ( "CPUID\n\t"
				"RDTSC\n\t"
				"mov %%edx, %0\n\t"
				"mov %%eax, %1\n\t": "=r" (cycles_high1), "=r" (cycles_low1)::
				"%rax", "%rbx", "%rcx", "%rdx");

		std::this_thread::sleep_for(std::chrono::microseconds(1000));

		asm volatile ( "RDTSCP\n\t"
				"mov %%edx, %0\n\t"
				"mov %%eax, %1\n\t"
				"CPUID\n\t": "=r" (cycles_high2), "=r" (cycles_low2)::
				"%rax", "%rbx", "%rcx", "%rdx");
		auto end = Timer::now();

		int64_t endCycles = ((int64_t) cycles_high2 << 32) | cycles_low2;
		int64_t startCycles = ((int64_t) cycles_high1 << 32) | cycles_low1;

		auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() - nowOverhead;

		_tscFrequency = (((float) (endCycles - startCycles)) * 1000000000.f) / (float) nanoseconds;
		_tscTimeRatio = (float) nanoseconds / ((float) (endCycles - startCycles));

	}

	void CpuInfo::getAMDFeatures()
	{
		unsigned eax, ebx, edx, ecx;

		__get_cpuid(0x80000001, &eax, &ebx, &ecx, &edx);

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

		__get_cpuid(0x0000001, &eax, &ebx, &ecx, &edx);

		if (featureCheck(edx, 19))
		{
			_cacheLineSize = getRegisterValue(ebx, 8, 0xFF);
		}

		if (featureCheck(edx, 28))
		{
			_hyperThreading = true;
			_logicalProcessors = getRegisterValue(ebx, 16, 0xFF);
		}

		for (int i = 2; i <= 4; ++i)
		{
			__get_cpuid(0x80000000 + i, &eax, &ebx, &ecx, &edx);
			_processorBrand.append((char*) (&eax), 4);
			_processorBrand.append((char*) (&ebx), 4);
			_processorBrand.append((char*) (&ecx), 4);
			_processorBrand.append((char*) (&edx), 4);
		}

		__get_cpuid(0x80000001, &eax, &ebx, &ecx, &edx);
		addFeature(_instructionExtensions, "CMOV", edx, 15);
		addFeature(_instructionExtensions, "MMX", edx, 23);
		addFeature(_instructionExtensions, "MmxExt", edx, 22);

		__get_cpuid(0x1, &eax, &ebx, &ecx, &edx);
		_rdtscp = featureCheck(edx, 4);
		addFeature(_instructionExtensions, "SSE", edx, 25);
		addFeature(_instructionExtensions, "SSE2", edx, 26);
		addFeature(_instructionExtensions, "SSE3", ecx, 0);
		addFeature(_instructionExtensions, "SSSE3", ecx, 9);
		addFeature(_instructionExtensions, "SSE41", ecx, 19);
		addFeature(_instructionExtensions, "SSE42", ecx, 20);

		__get_cpuid(0x80000001, &eax, &ebx, &ecx, &edx);
		addFeature(_instructionExtensions, "SSE4A", ecx, 6);

		__get_cpuid(0x1, &eax, &ebx, &ecx, &edx);
		addFeature(_instructionExtensions, "FMA", ecx, 12);

		__get_cpuid(0x80000001, &eax, &ebx, &ecx, &edx);
		addFeature(_instructionExtensions, "FMA4", ecx, 16);

		__get_cpuid(0x1, &eax, &ebx, &ecx, &edx);
		addFeature(_instructionExtensions, "AES", ecx, 25);
		addFeature(_instructionExtensions, "AVX", ecx, 28);

		if (_maximumStandardFunction >= 7)
		{
			__get_cpuid(0x7, &eax, &ebx, &ecx, &edx);
		}
		else
		{
			eax = ebx = ecx = edx = 0;
		}
		addFeature(_instructionExtensions, "AVX2", ebx, 5);
		// Not supported by AMD
		addFeature(_instructionExtensions, "AVX512-F", 0, 0);
		addFeature(_instructionExtensions, "AVX512-BW", 0, 0);
		addFeature(_instructionExtensions, "AVX512-DQ", 0, 0);
		addFeature(_instructionExtensions, "AVX512-IFMA", 0, 0);
		addFeature(_instructionExtensions, "AVX512-PF", 0, 0);
		addFeature(_instructionExtensions, "AVX512-ER", 0, 0);
		addFeature(_instructionExtensions, "AVX512-CD", 0, 0);
		addFeature(_instructionExtensions, "AVX512-VBMI", 0, 0);
		//addFeature(_instructionExtensions, "AVX512-VBMI2", 0, 0);
		//addFeature(_instructionExtensions, "AVX512-VNNI", 0, 0);
		//addFeature(_instructionExtensions, "AVX512-4VNNIW", 0, 0);
		//addFeature(_instructionExtensions, "AVX512-BITALG", 0, 0);
		//addFeature(_instructionExtensions, "AVX512-VL", 0, 0);
		//addFeature(_instructionExtensions, "AVX512-VPOPCNTDQ", 0, 0);
		//addFeature(_instructionExtensions, "AVX512-4FMAPS", 0, 0);
		addFeature(_instructionExtensions, "SHA", 0, 0);
		addFeature(_instructionExtensions, "SGX", 0, 0);
		//addFeature(_instructionExtensions, "VAES", 0, 0);

		__get_cpuid(0x80000001, &eax, &ebx, &ecx, &edx);
		addFeature(_instructionExtensions, "XOP", ecx, 11);
		addFeature(_instructionExtensions, "3DNow", edx, 31);
		addFeature(_instructionExtensions, "3DNowExt", edx, 30);
		addFeature(_instructionExtensions, "3DNowPrefetch", ecx, 8);

		if (_maximumStandardFunction >= 7)
		{
			__get_cpuid(0x7, &eax, &ebx, &ecx, &edx);
		}
		else
		{
			eax = ebx = ecx = edx = 0;
		}

		addFeature(_instructionExtensions, "BMI1", ebx, 3);
		addFeature(_instructionExtensions, "BMI2", ebx, 8);

		__get_cpuid(0x80000001, &eax, &ebx, &ecx, &edx);
		addFeature(_instructionExtensions, "ABM", ecx, 5);

		__get_cpuid(0x1, &eax, &ebx, &ecx, &edx);
		addFeature(_instructionExtensions, "F16C", ecx, 29);
		addFeature(_instructionExtensions, "RDRAND", ecx, 30);

		if (_maximumExtendedFunction >= 0x80000008)
		{
			__get_cpuid(0x80000008, &eax, &ebx, &ecx, &edx);
			_physicalCores = getRegisterValue(ecx, 0, 0xFF) + 1;
		}

		if (_maximumExtendedFunction >= 0x80000005)
		{

			__get_cpuid(0x80000005, &eax, &ebx, &ecx, &edx);
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

			__get_cpuid(0x80000006, &eax, &ebx, &ecx, &edx);

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
			__get_cpuid(0x80000007, &eax, &ebx, &ecx, &edx);
			_turboBoost = featureCheck(edx, 9);
			_invariantTsc = featureCheck(edx, 8);
		}
	}

	Map<String, CpuFeature> CpuInfo::getInstructionSetSupport() const
	{
		Map<String, CpuFeature> returnMap;

		for (auto& feature : _instructionExtensions)
		{
			returnMap.emplace(feature.getName(), CpuFeature(feature));
		}
		return returnMap;
	}

	void CpuInfo::getIntelFeatures()
	{
		unsigned eax, ebx, edx, ecx;

		__get_cpuid(0x1, &eax, &ebx, &ecx, &edx);

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
			_logicalProcessors = getRegisterValue(ebx, 16, 0xFF);
		}
		else
		{
			_logicalProcessors = _physicalCores;
		}

		_rdtscp = featureCheck(edx, 4);

		addFeature(_instructionExtensions, "CMOV", edx, 15);
		addFeature(_instructionExtensions, "MMX", edx, 23);
		addFeature(_instructionExtensions, "MmxExt", 0, 0);	// Not supported by Intel
		addFeature(_instructionExtensions, "SSE", edx, 25);
		addFeature(_instructionExtensions, "SSE2", edx, 26);
		addFeature(_instructionExtensions, "SSE3", ecx, 0);
		addFeature(_instructionExtensions, "SSSE3", ecx, 9);
		addFeature(_instructionExtensions, "SSE41", ecx, 19);
		addFeature(_instructionExtensions, "SSE42", ecx, 20);
		addFeature(_instructionExtensions, "SSE4A", 0, 0);	// Not supported by Intel
		addFeature(_instructionExtensions, "FMA", ecx, 12);
		addFeature(_instructionExtensions, "FMA4", 0, 0);	// Not supported by Intel
		addFeature(_instructionExtensions, "AES", ecx, 25);
		addFeature(_instructionExtensions, "AVX", ecx, 28);

		if (_maximumStandardFunction >= 7)
		{
			__get_cpuid(0x7, &eax, &ebx, &ecx, &edx);
		}
		else
		{
			eax = ebx = ecx = edx = 0;
		}

		addFeature(_instructionExtensions, "AVX2", ebx, 5);
		addFeature(_instructionExtensions, "AVX512-F", ebx, 16);
		addFeature(_instructionExtensions, "AVX512-BW", ebx, 30);
		addFeature(_instructionExtensions, "AVX512-DQ", ebx, 17);
		addFeature(_instructionExtensions, "AVX512-IFMA", ebx, 21);
		addFeature(_instructionExtensions, "AVX512-PF", ebx, 26);
		addFeature(_instructionExtensions, "AVX512-ER", ebx, 27);
		addFeature(_instructionExtensions, "AVX512-CD", ebx, 28);
		addFeature(_instructionExtensions, "AVX512-VBMI", ecx, 1);

		// The bellow are not written in intel documentation, seen on wikipedia (CPUID)
		// its better to  disable for now
		//addFeature(_instructionExtensions, "AVX512-VBMI2", ecx, 6);
		//addFeature(_instructionExtensions, "AVX512-VNNI", ecx, 11);
		//addFeature(_instructionExtensions, "AVX512-4VNNIW", edx, 2);
		//addFeature(_instructionExtensions, "AVX512-BITALG", ecx, 12);
		//addFeature(_instructionExtensions, "AVX512-VPOPCNTDQ", ecx, 14);
		//addFeature(_instructionExtensions, "AVX512-4FMAPS", edx, 3);
		addFeature(_instructionExtensions, "SHA", ebx, 29);
		addFeature(_instructionExtensions, "SGX", ebx, 2);
		//addFeature(_instructionExtensions, "VAES", ecx, 9);

		addFeature(_instructionExtensions, "XOP", 0, 0);
		addFeature(_instructionExtensions, "3DNow", 0, 0);
		addFeature(_instructionExtensions, "3DNowExt", 0, 0);
		addFeature(_instructionExtensions, "3DNowPrefetch", 0, 0);
		addFeature(_instructionExtensions, "BMI", ebx, 3);
		addFeature(_instructionExtensions, "BMI2", ebx, 8);
		addFeature(_instructionExtensions, "ABM", 0, 0);

		__get_cpuid(0x1, &eax, &ebx, &ecx, &edx);
		addFeature(_instructionExtensions, "F16C", ecx, 29);
		addFeature(_instructionExtensions, "RDRAND", ecx, 30);

		for (int i = 2; i <= 4; ++i)
		{
			__get_cpuid(0x80000000 + i, &eax, &ebx, &ecx, &edx);
			_processorBrand.append((char*) (&eax), 4);
			_processorBrand.append((char*) (&ebx), 4);
			_processorBrand.append((char*) (&ecx), 4);
			_processorBrand.append((char*) (&edx), 4);
		}

		if (_maximumStandardFunction >= 0x6)
		{
			__get_cpuid(0x6, &eax, &ebx, &ecx, &edx);
			_turboBoost = featureCheck(eax, 1);
			_turboBoost3 = featureCheck(eax, 14);
		}

		if (_maximumStandardFunction >= 0x4)
		{

			int i = 0;

			while (true)
			{
				__asm__ volatile ("movl %0, %%ecx;"
						:
						: "r" (i));

				__get_cpuid(0x4, &eax, &ebx, &ecx, &edx);

				if (getRegisterValue(eax, 0, 0x1F) == 0) break;

				Cache tmpCache;

				tmpCache.name = "L" + std::to_string(getRegisterValue(eax, 5, 0x7)) + " "
				        + intelCacheTypes[getRegisterValue(eax, 0, 0x1F) < 4 ? getRegisterValue(eax, 0, 0x1F) : 0];

				tmpCache.size = (getRegisterValue(ebx, 22, 0x3FF) + 1) * (getRegisterValue(ebx, 12, 0x3FF) + 1)
				        * (getRegisterValue(ebx, 0, 0xFFF) + 1) * (getRegisterValue(ecx, 0, 0x7FFFFFFF) + 1);

				tmpCache.associativity = std::to_string((getRegisterValue(ebx, 22, 0x3FF) + 1)) + "-way";
				tmpCache.linesPerTag = (getRegisterValue(ebx, 12, 0x3FF) + 1);
				tmpCache.lineSize = (getRegisterValue(ebx, 0, 0xFFF) + 1);
				_caches.push_back(tmpCache);
				i++;
			}
		}
		if (_maximumExtendedFunction >= 0x80000007)
		{
			__get_cpuid(0x80000007, &eax, &ebx, &ecx, &edx);
			_invariantTsc = featureCheck(edx, 8);
		}

	}

} /* namespace Elpida */
