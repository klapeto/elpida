//
// Created by klapeto on 16/9/2023.
//

#ifndef ELPIDA_APPS_QT_OVERHEADSCALCULATOR_OVERHEADSCALCULATOR_HPP
#define ELPIDA_APPS_QT_OVERHEADSCALCULATOR_OVERHEADSCALCULATOR_HPP

#include "Elpida/Core/OverheadsInfo.hpp"
#include "Elpida/Core/Topology/TopologyInfo.hpp"

namespace Elpida {
	class TopologyInfo;
}

namespace Elpida::Application
{

	class OverheadsCalculator final
	{
	public:
		static OverheadsInfo CalculateOverheads(const TopologyInfo& topologyInfo);
		OverheadsCalculator() = delete;
	};

} // Application

#endif //ELPIDA_APPS_QT_OVERHEADSCALCULATOR_OVERHEADSCALCULATOR_HPP
