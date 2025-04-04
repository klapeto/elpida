//
// Created by klapeto on 4/3/2023.
//

#include "Elpida/Core/Topology/CpuCacheNode.hpp"

namespace Elpida
{
	CacheType CpuCacheNode::GetCacheType() const
	{
		return _cacheType;
	}

	Size CpuCacheNode::GetSize() const
	{
		return _size;
	}
	Size CpuCacheNode::GetLineSize() const
	{
		return _lineSize;
	}

	int CpuCacheNode::GetAssociativitySets() const
	{
		return _associativitySets;
	}

	bool CpuCacheNode::IsFullyAssociative() const
	{
		return _fullyAssociative;
	}

	CpuCacheNode::CpuCacheNode(
		NodeType type,
		CacheType cacheType,
		Size size,
		Size lineSize,
		int associativitySets,
		bool fullyAssociative)
		: TopologyNode(type),
		  _fullyAssociative(fullyAssociative),
		  _size(size),
		  _lineSize(lineSize),
		  _cacheType(cacheType),
		  _associativitySets(associativitySets)
	{

	}
} // Elpida