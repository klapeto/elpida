//
// Created by klapeto on 2/3/2023.
//

#ifndef _TOPOLOGYINFO_HPP_
#define _TOPOLOGYINFO_HPP_

#include "TopologyNode.hpp"
#include "Elpida/Topology/CpuKind.hpp"
#include "Elpida/Topology/CpuCacheNode.hpp"
#include "Elpida/Topology/ProcessingUnitNode.hpp"
#include "Elpida/Topology/NumaNode.hpp"

#include <memory>
#include <vector>

namespace Elpida
{

	class TopologyInfo final
	{
	 public:
		[[nodiscard]]
		const TopologyNode& GetRoot() const;

		[[nodiscard]]
		const std::vector<CpuKind>& GetCpuKinds() const;

		[[nodiscard]]
		std::size_t GetTotalLogicalCores() const;

		[[nodiscard]]
		std::size_t GetTotalPhysicalCores() const;

		[[nodiscard]]
		std::size_t GetTotalNumaNodes() const;

		[[nodiscard]]
		std::size_t GetTotalPackages() const;

		[[nodiscard]]
		const std::vector<std::reference_wrapper<const CpuCacheNode>>& GetAllCaches() const;

		[[nodiscard]]
		const std::vector<std::reference_wrapper<const NumaNode>>& GetAllNumaNodes() const;

		[[nodiscard]]
		const std::vector<std::reference_wrapper<const ProcessingUnitNode>>& GetAllProcessingUnits() const;

		TopologyInfo();
		TopologyInfo(const TopologyInfo&) = delete;
		TopologyInfo(TopologyInfo&&) noexcept = default;
		TopologyInfo& operator=(const TopologyInfo&) = delete;
		TopologyInfo& operator=(TopologyInfo&&) noexcept = default;
		~TopologyInfo() = default;
	 private:
		std::vector<CpuKind> _cpuKinds;
		std::vector<std::reference_wrapper<const CpuCacheNode>> _allCaches;
		std::vector<std::reference_wrapper<const NumaNode>> _allNumaNodes;
		std::vector<std::reference_wrapper<const ProcessingUnitNode>> _allProcessingUnits;
		std::unique_ptr<TopologyNode> _root;

		std::size_t _totalLogicalCores;
		std::size_t _totalPhysicalCores;
		std::size_t _totalNumaNodes;
		std::size_t _totalPackages;

		void accumulateCores(const TopologyNode& node);

		void processChildNode(const TopologyNode& node);
	};

} // Elpida

#endif //_TOPOLOGYINFO_HPP_
