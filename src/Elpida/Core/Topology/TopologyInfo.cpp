//
// Created by klapeto on 2/3/2023.
//

#include "Elpida/Core/Topology/TopologyInfo.hpp"
#include "Elpida/Core/Map.hpp"

namespace Elpida
{
	void TopologyInfo::accumulateCores(const TopologyNode& node)
	{
		for (const auto& child: node.GetMemoryChildren())
		{
			processChildNode(*child);
			accumulateCores(*child);
		}

		for (const auto& child: node.GetChildren())
		{
			processChildNode(*child);
			accumulateCores(*child);
		}
	}

	void TopologyInfo::processChildNode(const TopologyNode& node)
	{
		switch (node.GetType())
		{
		case NodeType::ProcessingUnit:
			_totalLogicalCores++;
			_allProcessingUnits.emplace_back(static_cast<const ProcessingUnitNode&>(node));
			break;
		case NodeType::Core:
			_totalPhysicalCores++;
			break;
		case NodeType::NumaDomain:
			_totalNumaNodes++;
			_allNumaNodes.emplace_back(static_cast<const NumaNode&>(node));
			break;
		case NodeType::Package:
			_totalPackages++;
			break;
		case NodeType::L5Cache:
		case NodeType::L4Cache:
		case NodeType::L3DCache:
		case NodeType::L3ICache:
		case NodeType::L2DCache:
		case NodeType::L2ICache:
		case NodeType::L1DCache:
		case NodeType::L1ICache:
			_allCaches.emplace_back(static_cast<const CpuCacheNode&>(node));
		default:
			break;
		}
	}

	const TopologyNode& TopologyInfo::GetRoot() const
	{
		return *_root;
	}

	const Vector<CpuKind>& TopologyInfo::GetCpuKinds() const
	{
		return _cpuKinds;
	}

	Size TopologyInfo::GetTotalLogicalCores() const
	{
		return _totalLogicalCores;
	}

	Size TopologyInfo::GetTotalPhysicalCores() const
	{
		return _totalPhysicalCores;
	}

	Size TopologyInfo::GetTotalNumaNodes() const
	{
		return _totalNumaNodes;
	}

	Size TopologyInfo::GetTotalPackages() const
	{
		return _totalPackages;
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

	TopologyInfo::TopologyInfo(Vector<CpuKind>&& cpuKinds, UniquePtr<TopologyNode> root)
		: _cpuKinds(std::move(cpuKinds)), _root(std::move(root)), _totalLogicalCores(0), _totalPhysicalCores(0), _totalNumaNodes(0), _totalPackages(0)
	{
		accumulateCores(*_root);
	}

} // Elpida