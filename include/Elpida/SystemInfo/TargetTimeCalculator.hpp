/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2021  Ioannis Panagiotopoulos
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
// Created by klapeto on 31/1/21.
//

#ifndef INCLUDE_ELPIDA_SYSTEMINFO_TARGETTIMECALCULATOR_HPP
#define INCLUDE_ELPIDA_SYSTEMINFO_TARGETTIMECALCULATOR_HPP

#include <cstddef>
#include <chrono>
#include <array>

#include "Elpida/Utilities/Duration.hpp"
#include "Elpida/Utilities/OptionalReference.hpp"

namespace Elpida
{
	/**
	 * Modernized/modified version of lmbench's "enough" algorithm
	 *
	 * @see https://www.usenix.org/legacy/publications/library/proceedings/sd96/full_papers/mcvoy.pdf
	 * @see https://github.com/intel/lmbench
	 */
	class TargetTimeCalculator final
	{
	public:
		[[nodiscard]] static bool test(Duration time, OptionalReference<bool> cancel);

		TargetTimeCalculator() = delete;
		~TargetTimeCalculator() = delete;
	private:
		template<typename TRep, typename TPeriod>
		using duration = std::chrono::duration<TRep, TPeriod>;

		static inline constexpr double ConstantTestPoints[] = { 1.015, 1.02, 1.035 };

		static inline constexpr double LowerErrorMarginMultiplier = 0.98;
		static inline constexpr double HigherErrorMarginMultiplier = 1.02;
		static inline constexpr double DiffVarianceMaxMargin = 0.0025;

		static inline constexpr size_t Tries = 11;
		static inline constexpr size_t InitialConstant = 10000;
		static inline constexpr size_t TooSoonTimeDivisor = 10;
		static inline constexpr size_t ConstantMultiplier = 10;

		template<typename TRep, typename TPeriod>
		static duration<TRep, TPeriod> measureTimeForIterations(size_t iterations, OptionalReference<bool> cancel);

		template<typename TRep, typename TPeriod>
		static size_t findIterationsForTime(const duration<TRep, TPeriod>& targetDuration, OptionalReference<bool> cancel);

		static bool isTimeAccurate(const Duration& constant, OptionalReference<bool> cancel);
	};
}

#endif //INCLUDE_ELPIDA_SYSTEMINFO_TARGETTIMECALCULATOR_HPP
