/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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