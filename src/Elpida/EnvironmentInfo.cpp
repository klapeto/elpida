//
// Created by klapeto on 4/3/2023.
//

#include "EnvironmentInfo.hpp"

namespace Elpida
{
	EnvironmentInfo::EnvironmentInfo(const OverheadsInfo& overheadsInfo, TopologyInfo&& topologyInfo)
		: _overheadsInfo(overheadsInfo), _topologyInfo(std::move(topologyInfo))
	{

	}
	const TopologyInfo& EnvironmentInfo::GetTopologyInfo() const
	{
		return _topologyInfo;
	}

	const OverheadsInfo& EnvironmentInfo::GetOverheadsInfo() const
	{
		return _overheadsInfo;
	}
} // Elpida