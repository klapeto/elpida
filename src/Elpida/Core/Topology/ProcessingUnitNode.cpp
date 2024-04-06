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

	ProcessingUnitNode::ProcessingUnitNode()
		: TopologyNode(NodeType::ProcessingUnit)
	{

	}
	const NumaNode& ProcessingUnitNode::GetNumaNode() const
	{
		return _numaNode.value();
	}

	void ProcessingUnitNode::SetLastCache(const CpuCacheNode& cacheNode)
	{
		_lastCache = cacheNode;
	}

	void ProcessingUnitNode::SetNumaNode(const NumaNode& numaNode)
	{
		_numaNode = numaNode;
	}

	void ProcessingUnitNode::SetCpuKind(const CpuKind& cpuKind)
	{
		_cpuKind = cpuKind;
	}

} // Elpida