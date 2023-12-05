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

		[[nodiscard]]
		const AllocatorFactory& GetAllocatorFactory() const;

		EnvironmentInfo(CpuInfo&& cpuInfo, MemoryInfo&& memoryInfo, OsInfo&& osInfo, TopologyInfo&& topologyInfo, TimingInfo&& overheadsInfo, UniquePtr<AllocatorFactory> allocatorFactory);
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

		UniquePtr<AllocatorFactory> _allocatorFactory;
	};

} // Elpida

#endif //ELPIDA_ENVIRONMENTINFO_HPP_
