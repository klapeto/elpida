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
 * Cpu.hpp
 *
 *  Created on: 10 Μαρ 2018
 *      Author: klapeto
 */

#ifndef ELPIDA_CPUINFO_HPP_
#define ELPIDA_CPUINFO_HPP_

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "CpuFeature.hpp"

namespace Elpida
{

	class CpuInfo final
	{
	public:

		std::unordered_map<std::string, std::string> FeaturesNames =
			{
				{ "CMOV", "Conditional move instructions" },
				{ "MMX", "MMX instructions support" },
				{ "MmxExt", "AMD extensions to MMX instructions" },
				{ "SSE", "SSE instructions support" },
				{ "SSE2", "SSE2 instructions support" },
				{ "SSE3", "SSE3 instructions support" },
				{ "SSSE3", "Supplemental SSE3 instructions support" },
				{ "SSE41", "SSE4.1 instructions support" },
				{ "SSE42", "SSE4.2 instructions support" },
				{ "SSE4A", "SSE4A instructions support" },
				{ "FMA", "FMA instructions support" },
				{ "FMA4", "Four-operand FMA instructions support" },
				{ "AES", "AES instructions support" },
				{ "AVX", "AVX instructions support" },
				{ "AVX2", "AVX2 instructions support" },
				{ "AVX512-F", "AVX512 Foundation" },
				{ "AVX512-BW", "AVX-512 Byte and Word instructions support" },
				{ "AVX512-DQ", "AVX-512 DWORD and QWORD instructions support" },
				{ "AVX512-IFMA", "AVX-512 Integer FMA instructions support" },
				{ "AVX512-PF", "AVX-512 Prefetch instructions support" },
				{ "AVX512-ER", "AVX-512 Exp. and Recp. instructions support" },
				{ "AVX512-CD", "AVX-512 Conflict Detection instructions support" },
				{ "AVX512-VBMI", "AVX-512 Vector BMI instructions support" },
				{ "AVX512-VBMI2", "AVX-512 Vector BMI2 instructions support" },
				{ "AVX512-VNNI", "AVX-512 Vector Neural Network instructions support" },
				{ "AVX512-4VNNIW", "AVX-512 4-register Neural Network instructions support" },
				{ "AVX512-BITALG", "AVX-512 BITALG instructions support" },
				{ "AVX512-VL", "AVX-512 Vector Length Extensions instructions support" },
				{ "AVX512-VPOPCNTDQ", "AVX-512 Vector Population Count D/Q instructions support" },
				{ "AVX512-VPCLMULQDQ", "AVX-512 CLMUL instruction set (VEX-256/EVEX) instructions support" },
				{ "AVX512-4FMAPS", "AVX-512 4-register Multiply Accumulation Single precision instructions support" },
				{ "AVX512-GFNI", "AVX-512 Galois Field instructions support" },
				{ "AVX512-VAES", "AVX-512 AES instruction set (VEX-256/EVEX) instructions support" },
				{ "SHA", "Intel SHA extensions support" },
				{ "XOP", "Extended operation support" },
				{ "3DNow", "3DNow! instruction support" },
				{ "3DNowExt", "AMD extensions to 3DNow! instructions" },
				{ "3DNowPrefetch", "PREFETCH and PREFETCHW instruction support" },
				{ "BMI1", "Bit manipulation group 1 instruction support" },
				{ "BMI2", "Bit manipulation group 2 instruction support" },
				{ "ABM", "Advanced bit manipulation" },
				{ "F16C", "Half-precision convert instruction support" },
				{ "RDRAND", "RDRAND (HW RNG) instruction support" },
				{ "SGX", "Intel Software Guard Extensions BMI1 instructions support" },
			};

		struct Cache
		{
			std::string name;
			std::string associativity;
			int size;
			int linesPerTag;
			int lineSize;
		};

		enum class Vendor
		{
			AMD, Intel, Unknown
		};

		float getTscTimeRatio() const
		{
			return _tscTimeRatio;
		}

		bool isInvariantTsc() const
		{
			return _invariantTsc;
		}

		float getTscFequency() const
		{
			return _tscFrequency;
		}

		int getCacheLineSize() const
		{
			return _cacheLineSize;
		}

		int getFamily() const
		{
			return _family;
		}

		bool isHyperThreading() const
		{
			return _hyperThreading;
		}

		const std::vector<CpuFeature>& getInstructionExtensions() const
		{
			return _instructionExtensions;
		}

		int getModel() const
		{
			return _model;
		}

		const std::string& getProcessorBrand() const
		{
			return _processorBrand;
		}

		bool isRdtscp() const
		{
			return _rdtscp;
		}

		int getStepping() const
		{
			return _stepping;
		}

		Vendor getVendor() const
		{
			return _vendor;
		}

		const std::string& getVendorString() const
		{
			return _vendorString;
		}

		bool isTurboBoost() const
		{
			return _turboBoost;
		}

		bool isTurboBoost3() const
		{
			return _turboBoost3;
		}

		const std::vector<Cache>& getCaches() const
		{
			return _caches;
		}

		std::unordered_map<std::string, CpuFeature> getInstructionSetSupport() const;

		CpuInfo(CpuInfo&&) = default;
		CpuInfo(const CpuInfo&) = default;
		CpuInfo& operator=(CpuInfo&&) = default;
		CpuInfo& operator=(const CpuInfo&) = default;

		CpuInfo();
		~CpuInfo();
	private:
		std::vector<CpuFeature> _instructionExtensions;
		std::vector<Cache> _caches;
		std::string _vendorString;
		std::string _processorBrand;

		Vendor _vendor;

		int _model;
		int _family;
		int _stepping;
		int _cacheLineSize;
		unsigned _maximumStandardFunction;
		unsigned _maximumExtendedFunction;
		unsigned _crystalClock;
		float _tscFrequency;
		float _tscTimeRatio;
		bool _hyperThreading;
		bool _rdtscp;
		bool _turboBoost;
		bool _turboBoost3;
		bool _invariantTsc;

		void getTscFrequency();
		void getAMDFeatures();
		void getIntelFeatures();
	};

} /* namespace Elpida */

#endif /* ELPIDA_CPUINFO_HPP_ */
