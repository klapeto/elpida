/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

//
// Created by klapeto on 16/9/2023.
//

#ifndef ELPIDA_TIMINGCALCULATOR_HPP
#define ELPIDA_TIMINGCALCULATOR_HPP

#include "Elpida/Core/TimingInfo.hpp"

namespace Elpida
{
	class TopologyInfo;

	class TimingCalculator final
	{
	public:
		static TimingInfo CalculateTiming();
		static Duration CalculateLoopOverheadFast();
		TimingCalculator() = delete;
	};

} // Application

#endif //ELPIDA_TIMINGCALCULATOR_HPP
