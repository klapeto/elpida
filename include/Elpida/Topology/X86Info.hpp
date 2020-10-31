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

#ifndef INCLUDE_ELPIDA_TOPOLOGY_X86INFO_HPP
#define INCLUDE_ELPIDA_TOPOLOGY_X86INFO_HPP

#include "CpuInfo.hpp"

namespace Elpida
{
	class X86Info : public CpuInfo
	{
	public:
		static inline const std::string stepping = "Stepping";
		static inline const std::string model = "Model";
		static inline const std::string family = "Family";
		static inline const std::string turboBoost = "Turbo Boost";
		static inline const std::string turboBoost3 = "Turbo Boost 3";

		X86Info();
		~X86Info() override = default;
	private:
		unsigned _maximumStandardFunction = 0;
		unsigned _maximumExtendedFunction = 0;
		bool _rdtscp = false;

		void getTscFrequency();
		void getAMDFeatures();
		void getIntelFeatures();

		void addFeature(const std::string& name, unsigned reg, unsigned bit);
	};

}

#endif //INCLUDE_ELPIDA_TOPOLOGY_X86INFO_HPP
