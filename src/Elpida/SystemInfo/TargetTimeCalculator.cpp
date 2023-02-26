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

#include "Elpida/SystemInfo/TargetTimeCalculator.hpp"

#include "Elpida/Utilities/Timer.hpp"
#include "Elpida/Utilities/Macros.hpp"
#include "Elpida/Common/OperationCanceledException.hpp"

#include <chrono>

namespace Elpida
{
	using namespace std::chrono;

	static void throwIfCancelRequested(OptionalReference<bool> cancel)
	{
		if (cancel.has_value() && cancel->get())
		{
			throw OperationCanceledException();
		}
	}

	// This (volatile) is used to trick the compiler from optimizing out the innerLoop()
	static volatile long Dummy = 0;

	static long** innerLoop(std::size_t iterations, long** ptr)
	{
		// Ping-Pong loop by derefercing a pointer that points to itself
		while (iterations-- > 0)
		{
			REPEAT_10(ptr = reinterpret_cast<long**>(*ptr));
		}
		return ptr;
	}

	template<typename TRep, typename TPeriod>
	TargetTimeCalculator::duration<TRep, TPeriod> TargetTimeCalculator::measureTimeForIterations(size_t iterations, OptionalReference<bool> cancel)
	{
		throwIfCancelRequested(cancel);
		// These variables are dummy and needed for dereferencing inside
		// the inner loop. It will Ping-Pong with it self
		long* dummy = reinterpret_cast<long*>(&dummy);    // Points to itself
		long** ptr = static_cast<long**>(&dummy);    // Points to dummy

		duration<TRep, TPeriod> minDuration(0);

		for (size_t i = 0; i < Tries; ++i)
		{
			throwIfCancelRequested(cancel);

			auto start = Timer::now();

			// perform the loop
			ptr = innerLoop(iterations, ptr);

			auto end = Timer::now();

			// Keep the lowest duration
			auto thisIterationDuration = duration_cast<duration<TRep, TPeriod>>(end - start);
			if (minDuration == duration<TRep, TPeriod>(0) || thisIterationDuration < minDuration)
			{
				minDuration = thisIterationDuration;
			}
		}

		// This is used to trick the compiler from optimizing out the innerLoop()
		Dummy += *((long*)ptr);

		return minDuration;
	}

	template<typename TRep, typename TPeriod>
	size_t TargetTimeCalculator::findIterationsForTime(const duration<TRep, TPeriod>& targetDuration, OptionalReference<bool> cancel)
	{
		throwIfCancelRequested(cancel);

		const auto tooSoon = duration_cast<duration<TRep, TPeriod>>(targetDuration / (TRep)TooSoonTimeDivisor);
		const auto lowerMargin = duration_cast<duration<TRep, TPeriod>>(LowerErrorMarginMultiplier * targetDuration);
		const auto higherMargin = duration_cast<duration<TRep, TPeriod>>(HigherErrorMarginMultiplier * targetDuration);

		// Start with InitialConstant
		auto currentConstant = InitialConstant;

		// measure the initial constant
		auto currentConstantDuration = measureTimeForIterations<TRep, TPeriod>(currentConstant, cancel);

		for (size_t i = 0; i < Tries; ++i)
		{
			throwIfCancelRequested(cancel);

			// Is this duration near the target duration?
			if (currentConstantDuration > lowerMargin
				&& currentConstantDuration < higherMargin)
			{
				// we found a good candidate
				return currentConstant;
			}

			if (currentConstantDuration < tooSoon)
			{
				// loop completes too soon. Increase the multiplier
				currentConstant *= ConstantMultiplier;
			}
			else
			{
				// We are close to a candidate
				double newConstant = currentConstant;

				// Find the approximate duration/iteration
				newConstant /= currentConstantDuration.count();

				// since we know the duration/iteration we can calculate an approximate iterations
				// we need to achieve the desired duration
				newConstant *= targetDuration.count();

				// increase it by 1 add more safety to next calculation
				currentConstant = newConstant + 1;
			}

			currentConstantDuration = measureTimeForIterations<TRep, TPeriod>(currentConstant, cancel);
		}

		return 0;
	}

	bool TargetTimeCalculator::isTimeAccurate(const Duration& constant, OptionalReference<bool> cancel)
	{
		throwIfCancelRequested(cancel);

		// Use the constant as time to find an actual constant
		auto currentConstant = findIterationsForTime(constant, cancel);

		// Failed to get a constant?
		if (currentConstant == 0) return false;

		// Get a measurement with the constant we got. We will use it as a baseline to
		// measure the accuracy
		auto baseline = measureTimeForIterations<Duration::rep, Duration::period>(currentConstant, cancel);

		// Test various variations for about equal time variation
		for (auto testPoint : ConstantTestPoints)
		{
			throwIfCancelRequested(cancel);
			// Find the time for this testPoint
			auto actualTime =
				measureTimeForIterations<Duration::rep, Duration::period>(currentConstant * testPoint, cancel);

			// What we should expect under ideal scaling
			auto expected = duration_cast<Duration>(baseline * testPoint);

			// find the difference between expected and actual
			auto diff = std::abs(actualTime.count() - expected.count());

			// If the diff variance is too much, this means this constant is not good enough
			if (diff / static_cast<double>(expected.count()) > DiffVarianceMaxMargin)
			{
				return false;
			}
		}

		// current constant passed all test points for the constant initially provided
		// which means we can use it
		return true;
	}

	bool TargetTimeCalculator::test(Duration time, OptionalReference<bool> cancel)
	{
		// Original code did not have comments and it was kinda hard to understand how it worked.
		// So on this modernized version I made, I added comments so that people will not waste
		// too much time understanding it.

		// Test a predefined array of constant possibilities
		// We need to find a constant that produces consistent results
		return isTimeAccurate(time, cancel);
	}
}