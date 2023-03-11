//
// Created by klapeto on 4/3/2023.
//

#include "Elpida/Core/EnvironmentInfo.hpp"

namespace Elpida
{
	EnvironmentInfo::EnvironmentInfo(const OverheadsInfo& overheadsInfo, TopologyInfo&& topologyInfo, UniquePtr<Allocator> allocator)
		: _overheadsInfo(overheadsInfo), _topologyInfo(std::move(topologyInfo)), _allocator(std::move(allocator))
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

	const Allocator& EnvironmentInfo::GetAllocator() const
	{
		return *_allocator;
	}
} // Elpida