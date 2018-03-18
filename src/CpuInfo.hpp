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
 * Cpu.hpp
 *
 *  Created on: 10 Μαρ 2018
 *      Author: klapeto
 */

#ifndef SRC_CPUINFO_HPP_
#define SRC_CPUINFO_HPP_

#include <iostream>
#include <string>
#include <vector>

#include "CpuFeature.hpp"
#include "Exportable.hpp"

namespace Elpida
{

	class CpuInfo: public Exportable
	{
		public:

			struct Cache {
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

			int getBaseFequency() const
			{
				return _baseFequency;
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

			int getLogicalProcessors() const
			{
				return _logicalProcessors;
			}

			int getModel() const
			{
				return _model;
			}

			int getPhysicalCores() const
			{
				return _physicalCores;
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

			void exportTo(std::ostream& output) const;

			static const CpuInfo& getCpuInfo()
			{
				static CpuInfo info;
				return info;
			}

			CpuInfo(CpuInfo&&) = default;
			CpuInfo(const CpuInfo&) = default;
			CpuInfo& operator=(CpuInfo&&) = default;
			CpuInfo& operator=(const CpuInfo&) = default;

		private:
			std::vector<CpuFeature> _instructionExtensions;
			std::vector<Cache> _caches;
			std::string _vendorString;
			std::string _processorBrand;

			Vendor _vendor;

			int _model;
			int _family;
			int _stepping;
			int _baseFequency;
			int _cacheLineSize;
			int _physicalCores;
			int _logicalProcessors;
			unsigned _maximumStandardFunction;
			unsigned _maximumExtendedFunction;
			bool _hyperThreading;
			bool _rdtscp;
			bool _turboBoost;
			bool _turboBoost3;

			CpuInfo();
			~CpuInfo();
			void getCpuFrequency();
			void getAMDFeatures();
			void getIntelFeatures();

	};

} /* namespace Elpida */

#endif /* SRC_CPUINFO_HPP_ */
