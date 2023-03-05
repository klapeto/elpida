//
// Created by klapeto on 5/3/2023.
//

#include "NumaNode.hpp"

#include <hwloc.h>

namespace Elpida
{
	NumaNode::NumaNode(std::optional<std::reference_wrapper<TopologyNode>> parent, const std::vector<CpuKind>& cpuKinds, void* rootObj, void* node)
		: TopologyNode(parent, cpuKinds, rootObj, node)
	{
		auto currentNode = (hwloc_obj_t)node;

		_localMemorySize = currentNode->attr->numanode.local_memory;

		for (std::size_t i = 0; i < currentNode->attr->numanode.page_types_len; ++i)
		{
			auto type = currentNode->attr->numanode.page_types[i];
			_memoryPageTypes.emplace_back(type.size, type.count);
		}
	}

	const std::vector<MemoryPageType>& NumaNode::GetMemoryPageTypes() const
	{
		return _memoryPageTypes;
	}

	std::size_t NumaNode::GetLocalMemorySize() const
	{
		return _localMemorySize;
	}
} // Elpida