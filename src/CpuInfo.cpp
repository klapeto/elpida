/**************************************************************************
 *   elpida - CPU benchmark tool
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

#include "CpuInfo.hpp"

#include <cpuid.h>
#include <unistd.h>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <unordered_map>

#include "Utilities/TextColumn.hpp"
#include "Utilities/TextTable.hpp"

#define columnWidth 20
#define out(prefix, value)	output << std::left  <<std::setw(columnWidth) << prefix  << std::setw(columnWidth) << value << _newLine
#define featureCheck(reg, bit) (((reg & (1 << bit)) != 0))
#define getRegisterValue(reg, bitOffset, bits) ((reg >> bitOffset) & bits)
#define addFeature(category, name, reg, bit) category.push_back( { name, _featuresNames[name], featureCheck(reg, bit) })

#define vendorByteSize 12

namespace Elpida
{

	std::unordered_map<std::string, std::string> _featuresNames = {
			{ "CMOV", "Conditional move instructions" },
			{ "MMX", "MMX instruction support" },
			{ "MmxExt", "AMD extensions to MMX instructions" },
			{ "SSE", "SSE instruction support" },
			{ "SSE2", "SSE2 instruction support" },
			{ "SSE3", "SSE3 instruction support" },
			{ "SSSE3", "Supplemental SSE3 instruction support" },
			{ "SSE41", "SSE4.1 instruction support" },
			{ "SSE42", "SSE4.2 instruction support" },
			{ "SSE4A", "SSE4A instruction support" },
			{ "FMA", "FMA instruction support" },
			{ "FMA4", "Four-operand FMA instruction support" },
			{ "AES", "AES instruction support" },
			{ "AVX", "AVX instruction support" },
			{ "AVX2", "AVX2 instruction support" },
			{ "XOP", "Extended operation support" },
			{ "3DNow", "3DNow! instruction support" },
			{ "3DNowExt", "AMD extensions to 3DNow! instructions" },
			{ "3DNowPrefetch", "PREFETCH and PREFETCHW instruction support" },
			{ "BMI1", "Bit manipulation group 1 instruction support" },
			{ "BMI2", "Bit manipulation group 2 instruction support" },
			{ "ABM", "Advanced bit manipulation" },
			{ "F16C", "Half-precision convert instruction support" },
			{ "RDRAND", "RDRAND (HW RNG) instruction support" } };

	CpuInfo::CpuInfo() :
			_vendor(Vendor::Unknown), _model(-1), _family(-1), _baseFequency(0), _cacheLineSize(-1), _physicalCores(1), _logicalProcessors(
					1), _maximumStandardFunction(0), _maximumExtendedFunction(0), _hyperThreading(false), _rdtscp(false)
	{

		setPadding(4);

		if (__get_cpuid_max(0, nullptr) == 0)
		{
			return;
		}

		unsigned eax, ebx, edx, ecx;

		__get_cpuid(0x0, &eax, &ebx, &ecx, &edx);
		_maximumStandardFunction = eax;

		__get_cpuid(0x80000000, &eax, &ebx, &ecx, &edx);
		_maximumExtendedFunction = eax;
		_vendorString.append((char*) &ebx, vendorByteSize);

		if (_vendorString == "AuthenticAMD")
		{
			_vendor = Vendor::AMD;
		}
		else if (_vendorString == "GenuineIntel")
		{
			_vendor = Vendor::Intel;
		}

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
		getCpuFrequency();
	}

	CpuInfo::~CpuInfo()
	{

	}

	void CpuInfo::exportTo(std::ostream& output) const
	{
		output << _newLine;
		output << "======================================" << _newLine;
		output << "CPU Info" << _newLine;
		output << "======================================" << _newLine;
		out("Vendor:", _vendorString);
		out("Model:", _processorBrand);
		out("Base Frequency:", std::to_string(_baseFequency) + " MHz");
		out("ModelID:", _model);
		out("Family:", _family);
		out("Stepping:", _stepping);
		out("Hyper Threading:", (_hyperThreading ? "true" : "false"));
		out("Logical Cores:", _logicalProcessors);
		out("Physical Cores:", _physicalCores);

		output << _newLine << _newLine << "CPU Instructions Extensions:" << _newLine;
		TextTable<3> outputTable =
				{ TextColumn("Description", 50), TextColumn("Short", 15), TextColumn("Supported", 10) };

		outputTable.setPadding(4);
		for (auto instruction : _instructionExtensions)
		{
			outputTable.addRow(
					{
							instruction.getDescription(),
							instruction.getName(),
							(instruction.isSupported() ? "true" : "false") });
		}
		outputTable.exportTo(output);
	}

	void CpuInfo::getCpuFrequency()
	{
		unsigned long long int startCycles, endCycles;

		auto start = std::chrono::high_resolution_clock::now();
		__asm__ volatile ( "mfence\n"
				"rdtsc" : "=A" (startCycles));

		usleep(200);

		__asm__ volatile (
				"rdtsc" : "=A" (endCycles));
		auto end = std::chrono::high_resolution_clock::now();

		auto microSeconds = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

		_baseFequency = ((int) (endCycles - startCycles) / microSeconds.count());

	}

	void CpuInfo::getAMDFeatures()
	{
		unsigned eax, ebx, edx, ecx;

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
		addFeature(_instructionExtensions, "AVX", ecx, 25);

		if (_maximumStandardFunction >= 7)
		{
			__get_cpuid(0x7, &eax, &ebx, &ecx, &edx);
			addFeature(_instructionExtensions, "AVX2", ebx, 5);
		}

		__get_cpuid(0x80000001, &eax, &ebx, &ecx, &edx);
		addFeature(_instructionExtensions, "XOP", ecx, 11);
		addFeature(_instructionExtensions, "3DNow", edx, 31);
		addFeature(_instructionExtensions, "3DNowExt", edx, 30);
		addFeature(_instructionExtensions, "3DNowPrefetch", ecx, 8);

		if (_maximumStandardFunction >= 7)
		{
			__get_cpuid(0x7, &eax, &ebx, &ecx, &edx);
			addFeature(_instructionExtensions, "BMI1", ebx, 3);
			addFeature(_instructionExtensions, "BMI2", ebx, 8);
		}

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
	}

	void CpuInfo::getIntelFeatures()
	{
		unsigned eax, ebx, edx, ecx;
		__get_cpuid(0x1, &eax, &ebx, &ecx, &edx);
		_rdtscp = featureCheck(edx, 4);

	}

} /* namespace Elpida */

