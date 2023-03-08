//
// Created by klapeto on 5/3/2023.
//

#include "Elpida/Topology/ProcessingUnitNode.hpp"

#include <hwloc.h>

namespace Elpida
{
	ProcessingUnitNode::ProcessingUnitNode(Optional<Ref<TopologyNode>> parent, const Vector<CpuKind>& cpuKinds, void* rootObj, void* node)
		: TopologyNode(parent, cpuKinds, rootObj, node)
	{

		auto currentNode = (hwloc_obj_t)node;
		auto kindIndex = hwloc_cpukinds_get_by_cpuset((hwloc_topology_t)rootObj, currentNode->cpuset, 0);

		if (kindIndex != -1)
		{
			_cpuKind = cpuKinds.at(kindIndex);
		}

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
} // Elpida