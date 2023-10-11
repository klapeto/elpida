//
// Created by klapeto on 16/9/2023.
//

#ifndef ELPIDA_TIMINGCALCULATOR_HPP
#define ELPIDA_TIMINGCALCULATOR_HPP

#include "Elpida/Core/Duration.hpp"
#include "Elpida/Core/TimingInfo.hpp"
#include "Elpida/Core/Topology/TopologyInfo.hpp"

namespace Elpida
{

	class TopologyInfo;

	class TimingCalculator final
	{
	public:
		static TimingInfo CalculateTiming();
		TimingCalculator() = delete;
	};

} // Application

#endif //ELPIDA_TIMINGCALCULATOR_HPP
