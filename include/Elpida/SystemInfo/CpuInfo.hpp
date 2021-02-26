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
 * Cpu.hpp
 *
 *  Created on: 10 Μαρ 2018
 *      Author: klapeto
 */

#ifndef ELPIDA_CPUINFO_HPP_
#define ELPIDA_CPUINFO_HPP_

#include <string>
#include <vector>
#include <unordered_map>

#include "CpuFeature.hpp"
#include "CpuCache.hpp"

namespace Elpida
{

	class CpuInfo
	{
	public:

		static const CpuInfo& get();

		[[nodiscard]] const std::vector<CpuFeature>& getFeatures() const
		{
			return _features;
		}

		[[nodiscard]] const std::string& getProcessorBrand() const
		{
			return _processorBrand;
		}

		[[nodiscard]] const std::string& getVendorString() const
		{
			return _vendorString;
		}

		[[nodiscard]] const std::vector<CpuCache>& getCaches() const
		{
			return _caches;
		}

		[[nodiscard]] float getFrequency() const
		{
			return _frequency;
		}

		[[nodiscard]] bool isSmt() const
		{
			return _smt;
		}

		[[nodiscard]] const std::unordered_map<std::string, std::string>& getAdditionalInformation() const
		{
			return _additionalInformation;
		}

		CpuInfo(CpuInfo&&) = default;
		CpuInfo(const CpuInfo&) = default;
		CpuInfo& operator=(CpuInfo&&) = default;
		CpuInfo& operator=(const CpuInfo&) = default;

		virtual ~CpuInfo() = default;
	protected:
		CpuInfo() = default;

		std::vector<CpuFeature> _features;
		std::unordered_map<std::string, std::string> _additionalInformation;
		std::vector<CpuCache> _caches;
		std::string _vendorString;
		std::string _processorBrand;

		float _frequency = 0.0;

		bool _smt = false;
	};

} /* namespace Elpida */

#endif /* ELPIDA_CPUINFO_HPP_ */
