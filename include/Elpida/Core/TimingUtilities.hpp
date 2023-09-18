//
// Created by klapeto on 10/9/2023.
//

#ifndef ELPIDA_SRC_ELPIDA_CORE_TIMINGUTILTIES_HPP
#define ELPIDA_SRC_ELPIDA_CORE_TIMINGUTILTIES_HPP

#include "Elpida/Core/Duration.hpp"
#include "Elpida/Core/ElpidaException.hpp"
#include "Elpida/Core/Iterations.hpp"

#include <cmath>
#include <cstdint>
#include <iostream>
#include <limits>

namespace Elpida
{
	class TimingUtilities
	{
	public:
		template<typename TCallable>
		static Iterations GetIterationsNeededForExecutionTime(
			Duration targetDuration,
			Duration nowOverhead,
			Duration loopOverhead,
			TCallable callable)
		{
			const double marginOfError = 0.02;
			const Duration lowerBound = Duration(targetDuration * (1.0 - marginOfError));
			const Duration upperBound = Duration(targetDuration * (1.0 + marginOfError));

			Iterations iterations = 1;

			auto currentDuration = Duration::zero();

			while (true)
			{
				auto start = Timer::now();
				callable(iterations);
				auto end = Timer::now();

				currentDuration = ToDuration(end - start) - nowOverhead - (loopOverhead * iterations);
				if (currentDuration.count() < targetDuration.count() / 10.0)
				{
					iterations *= 10;
					continue;
				}

				if (currentDuration > lowerBound && currentDuration < upperBound)
				{
					break;
				}
				if (iterations == 1 && currentDuration > targetDuration)
				{
					break;
				}
				auto ratio = targetDuration.count() / currentDuration.count();

				iterations = std::ceil(iterations * ratio * (1.0 + (marginOfError / 2.0)));
			}

			return iterations;
		}
	};

} // Elpida

#endif //ELPIDA_SRC_ELPIDA_CORE_TIMINGUTILTIES_HPP
