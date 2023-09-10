//
// Created by klapeto on 10/9/2023.
//

#ifndef ELPIDA_SRC_ELPIDA_CORE_TIMINGUTILTIES_HPP
#define ELPIDA_SRC_ELPIDA_CORE_TIMINGUTILTIES_HPP

#include "Elpida/Core/Duration.hpp"
#include "Elpida/Core/Size.hpp"

#include <cmath>

namespace Elpida
{

	class TimingUtilities
	{
	public:
		template<typename TCallable>
		static Size GetIterationsNeededForExecutionTime(Duration targetDuration,
			Duration nowOverhead,
			Duration loopOverhead,
			TCallable callable)
		{
			Size iterations = 1;
			auto currentDuration = Duration::zero();

			while (true)
			{
				auto start = Timer::now();
				callable(iterations);
				auto end = Timer::now();

				currentDuration = ToDuration(end - start) - nowOverhead - (loopOverhead * iterations);

				if (currentDuration > (targetDuration * 0.98) && currentDuration < (targetDuration * 1.02))
				{
					break;
				}
				iterations =
					std::ceil((((double)iterations * targetDuration.count()) / currentDuration.count()) * 1.005);
			}
			return iterations;
		}
	};

} // Elpida

#endif //ELPIDA_SRC_ELPIDA_CORE_TIMINGUTILTIES_HPP
