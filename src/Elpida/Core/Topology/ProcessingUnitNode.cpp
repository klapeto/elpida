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