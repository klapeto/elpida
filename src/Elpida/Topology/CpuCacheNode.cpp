//
// Created by klapeto on 4/3/2023.
//

#include "CpuCacheNode.hpp"

#include <hwloc.h>

namespace Elpida
{
	CpuCacheNode::CpuCacheNode(std::optional<std::reference_wrapper<TopologyNode>> parent, const std::vector<CpuKind>& cpuKinds, void* rootObj, void* node)
		: TopologyNode(parent, cpuKinds, rootObj, node)
	{
		auto currentNode = (hwloc_obj_t)node;

		_size = currentNode->attr->cache.size;
		_associativitySets = currentNode->attr->cache.associativity;
		_lineSize = currentNode->attr->cache.linesize;

		if (_lineSize == 0)
		{
			_lineSize = 64; // Assume the usual and hope for the best.
		}

		if (_associativitySets == -1)
		{
			_fullyAssociative = true;
		}

		switch (currentNode->attr->cache.type)
		{
		case HWLOC_OBJ_CACHE_UNIFIED:
			_cacheType = CacheType::Unified;
			break;
		case HWLOC_OBJ_CACHE_DATA:
			_cacheType = CacheType::Data;
			break;
		case HWLOC_OBJ_CACHE_INSTRUCTION:
			_cacheType = CacheType::Instruction;
			break;
		}

	}
	CacheType CpuCacheNode::GetCacheType() const
	{
		return _cacheType;
	}

	std::size_t CpuCacheNode::GetSize() const
	{
		return _size;
	}
	std::size_t CpuCacheNode::GetLineSize() const
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
} // Elpida