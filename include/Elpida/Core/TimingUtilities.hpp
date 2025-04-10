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
#include <limits>

namespace Elpida
{
	class TimingUtilities
	{
	private:
		static Iterations CalculateNextIterations(Iterations previousIterations, double ratio, double marginOfError)
		{
			std::uint64_t thisIterations = std::ceil(previousIterations * ratio * (1.0 + (marginOfError / 2.0)));
			Iterations returnIterations = thisIterations;

			if (returnIterations != thisIterations)
			{
				throw ElpidaException("Iteration overflow detected! Please report this to Elpida and mention your system specs.");
			}
			return returnIterations;
		}
	public:
		template<typename TCallable>
		static Iterations GetIterationsNeededForExecutionTime(
				Duration targetDuration,
				Duration nowOverhead,
				Duration loopOverhead,
				TCallable callable)
		{
			Iterations iterations = 1;

			// Attempt 5 times to get the iterations
			for (int i = 1; i <= 5; ++i)
			{
				double marginOfError = 0.02 * i;
				Duration lowerBound = Duration(targetDuration * (1.0 - marginOfError));
				Duration upperBound = Duration(targetDuration * (1.0 + marginOfError));

				auto currentDuration = Duration::zero();

				auto attempts = 4;

				while (attempts > 0)
				{
					auto start = Timer::now();
					callable(iterations);
					auto end = Timer::now();

					currentDuration = ToDuration(end - start) - nowOverhead - (loopOverhead * iterations);
					if (currentDuration.count() < targetDuration.count() / 10.0)
					{
						iterations = CalculateNextIterations(iterations, 10.0, marginOfError);
						continue;
					}

					if (currentDuration > lowerBound && currentDuration < upperBound)
					{
						return iterations;
					}
					if (iterations == 1 && currentDuration > targetDuration)
					{
						return iterations;
					}
					attempts--;
					auto ratio = targetDuration.count() / currentDuration.count();

					iterations = CalculateNextIterations(iterations, ratio, marginOfError);
				}
			}

			throw ElpidaException("This system has too unstable timing! Elpida cannot get consistent timing within 1% margin.");
		}

		template<typename TCallable, typename TPrepare, typename TAdditionalOffset>
		static std::tuple<Iterations, Duration> ExecuteMinimumTime(
				Duration minimumDuration,
				Duration nowOverhead,
				Duration loopOverhead,
				TPrepare prepare,
				TAdditionalOffset additionalTimeOffset,
				TCallable callable)
		{
			const double marginOfError = 0.05;
			Iterations iterations = 1;

			auto currentDuration = Duration::zero();

			while (true)
			{
				prepare(iterations);
				auto start = Timer::now();
				callable(iterations);
				auto end = Timer::now();

				currentDuration = additionalTimeOffset(
						ToDuration(end - start) - nowOverhead - (loopOverhead * iterations));

				if (currentDuration > minimumDuration)
				{
					break;
				}
				if (currentDuration.count() < minimumDuration.count() / 10.0)
				{
					iterations = CalculateNextIterations(iterations, 10.0, marginOfError);
					continue;
				}

				auto ratio = minimumDuration.count() / currentDuration.count();

				iterations = CalculateNextIterations(iterations, ratio, marginOfError);
			}

			return { iterations, currentDuration };
		}

		template<typename TCallable, typename TPrepare, typename TAdditionalOffset>
		static std::tuple<Iterations, Duration> GetMinimumIterationsAndDurationNeededForExecutionTime(
				Duration targetDuration,
				Duration nowOverhead,
				Duration loopOverhead,
				TPrepare prepare,
				TAdditionalOffset additionalTimeOffset,
				TCallable callable)
		{
			Iterations iterations = 1;
			auto currentDuration = Duration::zero();

			// Attempt 5 times to get the iterations
			for (int i = 1; i <= 5; ++i)
			{
				double marginOfError = 0.05 * i;
				Duration lowerBound = Duration(targetDuration * (1.0 - marginOfError));
				Duration upperBound = Duration(targetDuration * (1.0 + marginOfError));

				auto attempts = 4;

				while (attempts > 0)
				{
					prepare(iterations);
					auto start = Timer::now();
					callable(iterations);
					auto end = Timer::now();

					currentDuration = additionalTimeOffset(
							ToDuration(end - start) - nowOverhead - (loopOverhead * iterations));

					if (currentDuration >= targetDuration)
					{
						return { iterations, currentDuration };
					}

					if (currentDuration.count() < targetDuration.count() / 10.0)
					{
						iterations = CalculateNextIterations(iterations, 10.0, marginOfError);
						continue;
					}

					if (currentDuration > lowerBound && currentDuration < upperBound)
					{
						return { iterations, currentDuration };
					}
					if (iterations == 1 && currentDuration >= targetDuration)
					{
						return { iterations, currentDuration };
					}

					attempts--;
					auto ratio = targetDuration.count() / currentDuration.count();

					iterations = CalculateNextIterations(iterations, ratio, marginOfError);
				}
			}

			return { iterations, currentDuration };
		}

		template<typename TCallable, typename TPrepare, typename TAdditionalOffset>
		static Iterations GetMinimumIterationsNeededForExecutionTime(
				Duration targetDuration,
				Duration nowOverhead,
				Duration loopOverhead,
				TPrepare prepare,
				TAdditionalOffset additionalTimeOffset,
				TCallable callable)
		{
			const double marginOfError = 0.05;
			const Duration lowerBound = Duration(targetDuration * (1.0 - marginOfError));
			const Duration upperBound = Duration(targetDuration * (1.0 + marginOfError));

			Iterations iterations = 1;

			auto currentDuration = Duration::zero();

			while (true)
			{
				prepare(iterations);
				auto start = Timer::now();
				callable(iterations);
				auto end = Timer::now();

				currentDuration = additionalTimeOffset(
						ToDuration(end - start) - nowOverhead - (loopOverhead * iterations));

				if (currentDuration >= targetDuration)
				{
					break;
				}
				if (currentDuration.count() < targetDuration.count() / 10.0)
				{
					iterations = CalculateNextIterations(iterations, 10.0, marginOfError);
					continue;
				}

				if (currentDuration > lowerBound && currentDuration < upperBound)
				{
					break;
				}
				if (iterations == 1 && currentDuration >= targetDuration)
				{
					break;
				}
				auto ratio = targetDuration.count() / currentDuration.count();

				iterations = CalculateNextIterations(iterations, ratio, marginOfError);
			}

			return iterations;
		}
	};

} // Elpida

#endif //ELPIDA_SRC_ELPIDA_CORE_TIMINGUTILTIES_HPP
