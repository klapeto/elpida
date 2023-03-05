//
// Created by klapeto on 5/3/2023.
//

#ifndef _PROCESSINGUNITNODE_HPP_
#define _PROCESSINGUNITNODE_HPP_

#include "Elpida/Topology/TopologyNode.hpp"
#include "Elpida/Topology/NumaNode.hpp"
#include "Elpida/Topology/CpuCacheNode.hpp"

namespace Elpida
{

	class ProcessingUnitNode : public TopologyNode
	{
	 public:

		[[nodiscard]]
		std::optional<std::reference_wrapper<const CpuKind>> GetCpuKind() const;

		[[nodiscard]]
		std::optional<std::reference_wrapper<const CpuCacheNode>> GetLastCache() const;

		[[nodiscard]]
		std::optional<std::reference_wrapper<const NumaNode>> GetNumaNode() const;

		ProcessingUnitNode(const ProcessingUnitNode&) = delete;
		ProcessingUnitNode(ProcessingUnitNode&&) noexcept = delete;
		ProcessingUnitNode& operator=(const ProcessingUnitNode&) = delete;
		ProcessingUnitNode& operator=(ProcessingUnitNode&&) noexcept = delete;
		~ProcessingUnitNode() override = default;
	 private:
		ProcessingUnitNode(std::optional<std::reference_wrapper<TopologyNode>> parent, const std::vector<CpuKind>& cpuKinds, void* rootObj, void* node);

		std::optional<std::reference_wrapper<const CpuCacheNode>> _lastCache;
		std::optional<std::reference_wrapper<const NumaNode>> _numaNode;

		std::optional<std::reference_wrapper<const CpuKind>> _cpuKind;

		friend class TopologyNode;
	};

} // Elpida

#endif //_PROCESSINGUNITNODE_HPP_
