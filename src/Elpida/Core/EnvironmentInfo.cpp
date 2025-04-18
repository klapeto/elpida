//
// Created by klapeto on 4/3/2023.
//

#include "Elpida/Core/EnvironmentInfo.hpp"

namespace Elpida
{
	EnvironmentInfo::EnvironmentInfo(CpuInfo&& cpuInfo, MemoryInfo&& memoryInfo, OsInfo&& osInfo,
			TopologyInfo&& topologyInfo, TimingInfo&& overheadsInfo)
			: _cpuInfo(std::move(cpuInfo)), _memoryInfo(std::move(memoryInfo)), _osInfo(std::move(osInfo)),
			  _topologyInfo(std::move(topologyInfo)), _overheadsInfo(overheadsInfo)
	{

	}

	const TopologyInfo& EnvironmentInfo::GetTopologyInfo() const
	{
		return _topologyInfo;
	}

	const TimingInfo& EnvironmentInfo::GetOverheadsInfo() const
	{
		return _overheadsInfo;
	}

	const CpuInfo& EnvironmentInfo::GetCpuInfo() const
	{
		return _cpuInfo;
	}

	const MemoryInfo& EnvironmentInfo::GetMemoryInfo() const
	{
		return _memoryInfo;
	}

	const OsInfo& EnvironmentInfo::GetOsInfo() const
	{
		return _osInfo;
	}
} // Elpida