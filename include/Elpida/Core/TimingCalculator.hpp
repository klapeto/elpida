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
		static TimingInfo CalculateTiming(const TopologyInfo& topologyInfo);
		TimingCalculator() = delete;
	};

} // Application

#endif //ELPIDA_TIMINGCALCULATOR_HPP
