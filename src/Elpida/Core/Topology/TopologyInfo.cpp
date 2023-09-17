//
// Created by klapeto on 2/3/2023.
//

#include "Elpida/Core/Topology/TopologyInfo.hpp"
#include "Elpida/Core/Map.hpp"

namespace Elpida
{
	const TopologyNode& TopologyInfo::GetRoot() const
	{
		return *_root;
	}

	const Vector<CpuKind>& TopologyInfo::GetCpuKinds() const
	{
		return _cpuKinds;
	}

	const Vector<Ref<const CpuCacheNode>>& TopologyInfo::GetAllCaches() const
	{
		return _allCaches;
	}

	const Vector<Ref<const NumaNode>>& TopologyInfo::GetAllNumaNodes() const
	{
		return _allNumaNodes;
	}

	const Vector<Ref<const ProcessingUnitNode>>& TopologyInfo::GetAllProcessingUnits() const
	{
		return _allProcessingUnits;
	}

	const Vector<Ref<const TopologyNode>>& TopologyInfo::GetAllCores() const
	{
		return _allCores;
	}

	TopologyInfo::TopologyInfo(UniquePtr<TopologyNode> root,
		Vector<CpuKind>&& cpuKinds,
		Vector<Ref<const CpuCacheNode>>&& allCaches,
		Vector<Ref<const NumaNode>>&& allNumaNodes,
		Vector<Ref<const TopologyNode>>&& allCores,
		Vector<Ref<const ProcessingUnitNode>> allProcessingUnits)
		: _cpuKinds(std::move(cpuKinds)),
		  _allCaches(std::move(allCaches)),
		  _allNumaNodes(std::move(allNumaNodes)),
		  _allCores(std::move(allCores)),
		  _allProcessingUnits(std::move(allProcessingUnits)),
		  _root(std::move(root))
	{

	}

} // Elpida