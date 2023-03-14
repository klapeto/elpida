//
// Created by klapeto on 4/3/2023.
//

#ifndef ELPIDA_ENVIRONMENTINFO_HPP_
#define ELPIDA_ENVIRONMENTINFO_HPP_

#include "Elpida/Core/OverheadsInfo.hpp"
#include "Elpida/Core/MemoryInfo.hpp"
#include "Elpida/Core/CpuInfo.hpp"
#include "Elpida/Core/Topology/TopologyInfo.hpp"
#include "Elpida/Core/Allocator.hpp"
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
		const OverheadsInfo& GetOverheadsInfo() const;

		[[nodiscard]]
		const Allocator& GetAllocator() const;

		EnvironmentInfo(CpuInfo&& cpuInfo, MemoryInfo&& memoryInfo, OsInfo&& osInfo, TopologyInfo&& topologyInfo, OverheadsInfo&& overheadsInfo, UniquePtr<Allocator> allocator);
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
		OverheadsInfo _overheadsInfo;

		UniquePtr<Allocator> _allocator;
	};

} // Elpida

#endif //ELPIDA_ENVIRONMENTINFO_HPP_
