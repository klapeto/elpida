//
// Created by klapeto on 5/3/2023.
//

#include "Elpida/Core/Topology/ProcessingUnitNode.hpp"

namespace Elpida
{
	Optional<Ref<const CpuKind>> ProcessingUnitNode::GetCpuKind() const
	{
		return _cpuKind;
	}
	Optional<Ref<const CpuCacheNode>> ProcessingUnitNode::GetLastCache() const
	{
		return _lastCache;
	}

	Optional<Ref<const NumaNode>> ProcessingUnitNode::GetNumaNode() const
	{
		return _numaNode;
	}

	ProcessingUnitNode::ProcessingUnitNode(Optional<Ref<const CpuKind>> cpuKind)
		: TopologyNode(NodeType::ProcessingUnit), _cpuKind(cpuKind)
	{

		auto numaNode = GetLastAncestor(NodeType::NumaDomain);
		if (numaNode.has_value())
		{
			_numaNode = static_cast<const NumaNode&>(numaNode.value().get());
		}

		constexpr const auto cacheTypes = NodeType::L5Cache
										  | NodeType::L4Cache
										  | NodeType::L3DCache
										  | NodeType::L3ICache
										  | NodeType::L2DCache
										  | NodeType::L2ICache
										  | NodeType::L1DCache
										  | NodeType::L1ICache;

		auto cacheNode = GetLastAncestor((NodeType)(cacheTypes));

		if (cacheNode.has_value())
		{
			_lastCache = static_cast<const CpuCacheNode&>(cacheNode.value().get());
		}
	}
} // Elpida