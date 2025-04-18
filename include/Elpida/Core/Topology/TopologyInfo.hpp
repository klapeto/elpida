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
		const Vector<Ref<const CpuCacheNode>>& GetAllCaches() const;

		[[nodiscard]]
		const Vector<Ref<const NumaNode>>& GetAllNumaNodes() const;

		[[nodiscard]]
		const Vector<Ref<const ProcessingUnitNode>>& GetAllProcessingUnits() const;

		[[nodiscard]]
		const Vector<Ref<const TopologyNode>>& GetAllCores() const;

		void PinProcessToProcessors(const std::vector<Ref<const ProcessingUnitNode>>& processors) const;
		void PinThreadToProcessor(unsigned int processorId) const;
		void ClearThreadPinning() const;

		TopologyInfo(UniquePtr<TopologyNode> root,
			Vector<CpuKind>&& cpuKinds,
			Vector<Ref<const CpuCacheNode>>&& allCaches,
			Vector<Ref<const NumaNode>>&& allNumaNodes,
			Vector<Ref<const TopologyNode>>&& allCores,
			Vector<Ref<const ProcessingUnitNode>> allProcessingUnits,
			void* topologyObj);
		TopologyInfo(const TopologyInfo&) = delete;
		TopologyInfo(TopologyInfo&&) noexcept ;
		TopologyInfo& operator=(const TopologyInfo&) = delete;
		TopologyInfo& operator=(TopologyInfo&&) noexcept;
		~TopologyInfo();
	 private:
		Vector<CpuKind> _cpuKinds;
		Vector<Ref<const CpuCacheNode>> _allCaches;
		Vector<Ref<const NumaNode>> _allNumaNodes;
		Vector<Ref<const TopologyNode>> _allCores;
		Vector<Ref<const ProcessingUnitNode>> _allProcessingUnits;
		UniquePtr<TopologyNode> _root;
		void* _topologyObj;
	};

} // Elpida

#endif //ELPIDA_TOPOLOGY_TOPOLOGYINFO_HPP_
