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

#ifndef ELPIDA_ENVIRONMENTINFO_HPP_
#define ELPIDA_ENVIRONMENTINFO_HPP_

#include "Elpida/Core/AllocatorFactory.hpp"
#include "Elpida/Core/TimingInfo.hpp"
#include "Elpida/Core/MemoryInfo.hpp"
#include "Elpida/Core/CpuInfo.hpp"
#include "Elpida/Core/Topology/TopologyInfo.hpp"
#include "Elpida/Core/UniquePtr.hpp"
#include "Elpida/Core/OsInfo.hpp"

namespace Elpida
{

	class EnvironmentInfo final
	{
	 public:

		[[nodiscard]]
		const CpuInfo& GetCpuInfo() const;

		[[nodiscard]]
		const MemoryInfo& GetMemoryInfo() const;

		[[nodiscard]]
		const OsInfo& GetOsInfo() const;

		[[nodiscard]]
		const TopologyInfo& GetTopologyInfo() const;

		[[nodiscard]]
		const TimingInfo& GetOverheadsInfo() const;

		EnvironmentInfo(CpuInfo&& cpuInfo,
				MemoryInfo&& memoryInfo,
				OsInfo&& osInfo,
				TopologyInfo&& topologyInfo,
				TimingInfo&& overheadsInfo);
		EnvironmentInfo(const EnvironmentInfo&) = delete;
		EnvironmentInfo(EnvironmentInfo&&) noexcept = default;
		EnvironmentInfo& operator=(const EnvironmentInfo&) = delete;
		EnvironmentInfo& operator=(EnvironmentInfo&&) noexcept = default;
		~EnvironmentInfo() = default;
	 private:
		CpuInfo _cpuInfo;
		MemoryInfo _memoryInfo;
		TopologyInfo _topologyInfo;
		OsInfo _osInfo;
		TimingInfo _overheadsInfo;
	};

} // Elpida

#endif //ELPIDA_ENVIRONMENTINFO_HPP_
