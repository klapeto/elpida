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

#include "Elpida/Core/TimingCalculator.hpp"

#include "Elpida/Core/Duration.hpp"
#include "Elpida/Core/TimingUtilities.hpp"
#include "Elpida/Core/TimingInfo.hpp"

namespace Elpida
{
	const constexpr Duration DefaultTestDuration = Seconds(1);

	Duration TimingCalculator::CalculateLoopOverheadFast()
	{
		return Seconds(
				1.0 / (double)TimingUtilities::GetIterationsNeededForExecutionTime(MilliSeconds(100),
						Seconds(0),
						Seconds(0),
						[](auto x)
						{
							while (x-- > 0);
						}));
	}

	static Duration CalculateLoopOverhead()
	{
		return Seconds(
			1.0 / (double)TimingUtilities::GetIterationsNeededForExecutionTime(DefaultTestDuration,
				Seconds(0),
				Seconds(0),
				[](auto x)
				{
				  while (x-- > 0);
				}));
	}

	static Duration CalculateNowOverhead(Duration loopOverhead)
	{
		return Seconds(
			1.0 / (double)TimingUtilities::GetIterationsNeededForExecutionTime(DefaultTestDuration,
				Seconds(0),
				loopOverhead,
				[](auto x)
				{
				  while (x-- > 0) std::chrono::high_resolution_clock::now();
				}));
	}

	TimingInfo TimingCalculator::CalculateTiming()
	{
		Duration loopOverhead;
		Duration nowOverhead;

		loopOverhead = CalculateLoopOverhead();
		nowOverhead = CalculateNowOverhead(loopOverhead);

		return TimingInfo(nowOverhead,
				loopOverhead,
				1.0 / loopOverhead.count());
	}
} // Application