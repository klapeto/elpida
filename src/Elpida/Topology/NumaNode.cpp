//
// Created by klapeto on 5/3/2023.
//

#include "Elpida/Topology/NumaNode.hpp"

#include <hwloc.h>

namespace Elpida
{
	NumaNode::NumaNode(Optional<Ref<TopologyNode>> parent, const Vector<CpuKind>& cpuKinds, void* rootObj, void* node)
		: TopologyNode(parent, cpuKinds, rootObj, node)
	{
		auto currentNode = (hwloc_obj_t)node;

		_localMemorySize = currentNode->attr->numanode.local_memory;

		for (Size i = 0; i < currentNode->attr->numanode.page_types_len; ++i)
		{
			auto type = currentNode->attr->numanode.page_types[i];
			_memoryPageTypes.emplace_back(type.size, type.count);
		}
	}

	const Vector<MemoryPageType>& NumaNode::GetMemoryPageTypes() const
	{
		return _memoryPageTypes;
	}

	Size NumaNode::GetLocalMemorySize() const
	{
		return _localMemorySize;
	}
} // Elpida