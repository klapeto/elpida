//
// Created by klapeto on 16/9/2023.
//

#ifndef ELPIDA_APPS_QT_OVERHEADSCALCULATOR_TIMINGCALCULATOR_HPP
#define ELPIDA_APPS_QT_OVERHEADSCALCULATOR_TIMINGCALCULATOR_HPP

#include "Elpida/Core/Duration.hpp"
#include "Elpida/Core/TimingInfo.hpp"
#include "Elpida/Core/Topology/TopologyInfo.hpp"

namespace Elpida
{
	class TopologyInfo;
}

namespace Elpida::Application
{

	class TimingCalculator final
	{
	public:
		static TimingInfo CalculateTiming(const TopologyInfo& topologyInfo);
		TimingCalculator() = delete;
	};

} // Application

#endif //ELPIDA_APPS_QT_OVERHEADSCALCULATOR_TIMINGCALCULATOR_HPP
