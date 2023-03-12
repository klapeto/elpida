//
// Created by klapeto on 5/3/2023.
//

#ifndef ELPIDA_TOPOLOGY_PROCESSINGUNITNODE_HPP_
#define ELPIDA_TOPOLOGY_PROCESSINGUNITNODE_HPP_

#include "Elpida/Core/Topology/TopologyNode.hpp"
#include "Elpida/Core/Topology/NumaNode.hpp"
#include "Elpida/Core/Topology/CpuCacheNode.hpp"
#include "Elpida/Core/Vector.hpp"
#include "Elpida/Core/Ref.hpp"
#include "Elpida/Core/Optional.hpp"

namespace Elpida
{

	class ProcessingUnitNode : public TopologyNode
	{
	 public:

		[[nodiscard]]
		const NumaNode& GetNumaNode() const;

		[[nodiscard]]
		Optional<Ref<const CpuKind>> GetCpuKind() const;

		[[nodiscard]]
		Optional<Ref<const CpuCacheNode>> GetLastCache() const;

		void SetLastCache(const CpuCacheNode& cacheNode);
		void SetNumaNode(const NumaNode& cacheNode);
		void SetCpuKind(const CpuKind& cacheNode);

		explicit ProcessingUnitNode();
		ProcessingUnitNode(const ProcessingUnitNode&) = delete;
		ProcessingUnitNode(ProcessingUnitNode&&) noexcept = delete;
		ProcessingUnitNode& operator=(const ProcessingUnitNode&) = delete;
		ProcessingUnitNode& operator=(ProcessingUnitNode&&) noexcept = delete;
		~ProcessingUnitNode() override = default;
	 private:
		Optional<Ref<const NumaNode>> _numaNode;
		Optional<Ref<const CpuCacheNode>> _lastCache;
		Optional<Ref<const CpuKind>> _cpuKind;
	};

} // Elpida

#endif //ELPIDA_TOPOLOGY_PROCESSINGUNITNODE_HPP_
