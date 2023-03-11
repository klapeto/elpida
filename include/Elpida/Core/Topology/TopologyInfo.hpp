//
// Created by klapeto on 2/3/2023.
//

#ifndef ELPIDA_TOPOLOGY_TOPOLOGYINFO_HPP_
#define ELPIDA_TOPOLOGY_TOPOLOGYINFO_HPP_

#include "Elpida/Core/Topology/TopologyNode.hpp"
#include "Elpida/Core/Topology/CpuKind.hpp"
#include "Elpida/Core/Topology/CpuCacheNode.hpp"
#include "Elpida/Core/Topology/ProcessingUnitNode.hpp"
#include "Elpida/Core/Topology/NumaNode.hpp"
#include "Elpida/Core/Vector.hpp"
#include "Elpida/Core/Size.hpp"
#include "Elpida/Core/Ref.hpp"
#include "Elpida/Core/UniquePtr.hpp"

namespace Elpida
{

	class TopologyInfo final
	{
	 public:
		[[nodiscard]]
		const TopologyNode& GetRoot() const;

		[[nodiscard]]
		const Vector<CpuKind>& GetCpuKinds() const;

		[[nodiscard]]
		Size GetTotalLogicalCores() const;

		[[nodiscard]]
		Size GetTotalPhysicalCores() const;

		[[nodiscard]]
		Size GetTotalNumaNodes() const;

		[[nodiscard]]
		Size GetTotalPackages() const;

		[[nodiscard]]
		const Vector<Ref<const CpuCacheNode>>& GetAllCaches() const;

		[[nodiscard]]
		const Vector<Ref<const NumaNode>>& GetAllNumaNodes() const;

		[[nodiscard]]
		const Vector<Ref<const ProcessingUnitNode>>& GetAllProcessingUnits() const;

		TopologyInfo(Vector<CpuKind>&& cpuKinds, UniquePtr<TopologyNode> root);
		TopologyInfo(const TopologyInfo&) = delete;
		TopologyInfo(TopologyInfo&&) noexcept = default;
		TopologyInfo& operator=(const TopologyInfo&) = delete;
		TopologyInfo& operator=(TopologyInfo&&) noexcept = default;
		~TopologyInfo() = default;
	 private:
		Vector<CpuKind> _cpuKinds;
		Vector<Ref<const CpuCacheNode>> _allCaches;
		Vector<Ref<const NumaNode>> _allNumaNodes;
		Vector<Ref<const ProcessingUnitNode>> _allProcessingUnits;
		UniquePtr<TopologyNode> _root;

		Size _totalLogicalCores;
		Size _totalPhysicalCores;
		Size _totalNumaNodes;
		Size _totalPackages;

		void accumulateCores(const TopologyNode& node);

		void processChildNode(const TopologyNode& node);
	};

} // Elpida

#endif //ELPIDA_TOPOLOGY_TOPOLOGYINFO_HPP_
