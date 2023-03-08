//
// Created by klapeto on 2/3/2023.
//

#include "Elpida/Topology/TopologyInfo.hpp"
#include "Elpida/Map.hpp"
#include "Elpida/String.hpp"

#include <hwloc.h>

namespace Elpida
{
	TopologyInfo::TopologyInfo()
	{
		hwloc_topology_t topology;
		try
		{
			hwloc_topology_init(&topology);
			hwloc_topology_set_all_types_filter(topology, HWLOC_TYPE_FILTER_KEEP_ALL);
			hwloc_topology_load(topology);

			auto kindsN = hwloc_cpukinds_get_nr(topology, 0);
			if (kindsN != -1)
			{
				for (auto i = 0; i < kindsN; ++i)
				{
					int efficiency = 0;
					unsigned int nrInfos = 0;
					hwloc_info_s* hwInfos;
					Map<String, String> infos;
					if (!hwloc_cpukinds_get_info(topology, i, nullptr, &efficiency, &nrInfos, &hwInfos, 0))
					{
						for (auto j = 0u; j < nrInfos; ++j)
						{
							infos.emplace(hwInfos[j].name, hwInfos[j].value);
						}
					}
					_cpuKinds.emplace_back(efficiency, std::move(infos));
				}
			}

			//hwloc_cpukinds_get_info(&topology,)

			_root = TopologyNode::Load(std::nullopt, _cpuKinds, topology, hwloc_get_root_obj(topology));
			_root->loadSiblings();    // Now its safe to attempt to recursively load all siblings
			_root->loadParents(std::nullopt);

			hwloc_topology_destroy(topology);

			processChildNode(*_root);
			accumulateCores(*_root);
		}
		catch (...)
		{
			hwloc_topology_destroy(topology);
			throw;
		}
	}

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

} // Elpida