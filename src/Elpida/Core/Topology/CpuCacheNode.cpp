/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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