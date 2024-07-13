//
// Created by klapeto on 5/3/2023.
//

#include "Elpida/Core/Topology/NumaNode.hpp"

namespace Elpida
{
	const Vector<MemoryPageType>& NumaNode::GetMemoryPageTypes() const
	{
		return _memoryPageTypes;
	}

	Size NumaNode::GetLocalMemorySize() const
	{
		return _localMemorySize;
	}

	NumaNode::NumaNode(
		Vector<MemoryPageType>&& memoryPageTypes,
		Size localMemorySize)
		: TopologyNode(NodeType::NumaDomain),
		_memoryPageTypes(std::move(memoryPageTypes)),
		_localMemorySize(localMemorySize)
	{

	}
} // Elpida