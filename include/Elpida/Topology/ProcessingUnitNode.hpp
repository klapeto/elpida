//
// Created by klapeto on 5/3/2023.
//

#ifndef ELPIDA_TOPOLOGY_PROCESSINGUNITNODE_HPP_
#define ELPIDA_TOPOLOGY_PROCESSINGUNITNODE_HPP_

#include "Elpida/Topology/TopologyNode.hpp"
#include "Elpida/Topology/NumaNode.hpp"
#include "Elpida/Topology/CpuCacheNode.hpp"
#include "Elpida/Vector.hpp"
#include "Elpida/Ref.hpp"
#include "Elpida/Optional.hpp"

namespace Elpida
{

	class ProcessingUnitNode : public TopologyNode
	{
	 public:

		[[nodiscard]]
		Optional<Ref<const CpuKind>> GetCpuKind() const;

		[[nodiscard]]
		Optional<Ref<const CpuCacheNode>> GetLastCache() const;

		[[nodiscard]]
		Optional<Ref<const NumaNode>> GetNumaNode() const;

		ProcessingUnitNode(const ProcessingUnitNode&) = delete;
		ProcessingUnitNode(ProcessingUnitNode&&) noexcept = delete;
		ProcessingUnitNode& operator=(const ProcessingUnitNode&) = delete;
		ProcessingUnitNode& operator=(ProcessingUnitNode&&) noexcept = delete;
		~ProcessingUnitNode() override = default;
	 private:
		ProcessingUnitNode(Optional<Ref<TopologyNode>> parent, const Vector<CpuKind>& cpuKinds, void* rootObj, void* node);

		Optional<Ref<const CpuCacheNode>> _lastCache;
		Optional<Ref<const NumaNode>> _numaNode;

		Optional<Ref<const CpuKind>> _cpuKind;

		friend class TopologyNode;
	};

} // Elpida

#endif //ELPIDA_TOPOLOGY_PROCESSINGUNITNODE_HPP_
