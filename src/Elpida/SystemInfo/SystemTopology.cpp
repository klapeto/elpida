/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2020  Ioannis Panagiotopoulos
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>
 *************************************************************************/

/*
 * SystemTopology.cpp
 *
 *  Created on: 12 Ιαν 2019
 *      Author: klapeto
 */

#include "Elpida/SystemInfo/SystemTopology.hpp"

#include "Elpida/Config.hpp"
#include "Elpida/SystemInfo/ProcessorNode.hpp"
#include <hwloc.h>

namespace Elpida
{
	SystemTopology::SystemTopology()
			: _root(nullptr),
			_depth(0),
			_totalLogicalCores(0),
			_totalPhysicalCores(0),
			_totalNumaNodes(0),
			_totalPackages(0)
	{
		reload();
	}

	void SystemTopology::reload()
	{
		hwloc_topology_t topo;
		hwloc_topology_init(&topo);
		hwloc_topology_set_all_types_filter(topo, HWLOC_TYPE_FILTER_KEEP_ALL);
		hwloc_topology_load(topo);

		//_depth = hwloc_topology_get_depth(topo);

		auto kindsN = hwloc_cpukinds_get_nr(topo, 0);
		if (kindsN != -1)
		{
			for (auto i = 0; i < kindsN; ++i)
			{
				int efficiency = 0;
				unsigned int nrInfos = 0;
				hwloc_info_s* hwInfos;
				std::unordered_map<std::string, std::string> infos;
				if (!hwloc_cpukinds_get_info(topo, i, nullptr, &efficiency, &nrInfos, &hwInfos, 0))
				{
					for (auto j = 0u; j < nrInfos; ++j)
					{
						infos.emplace(hwInfos[j].name, hwInfos[j].value);
					}
				}
				_cpuKinds.emplace_back(efficiency, std::move(infos));
			}
		}

		//hwloc_cpukinds_get_info(&topo,)

		_root = std::unique_ptr<ProcessorNode>(new ProcessorNode(nullptr, _cpuKinds, topo, hwloc_get_root_obj(topo)));
		_root->loadSiblings();    // Now its safe to attempt to recursively load all siblings
		_root->loadParents(nullptr);

		hwloc_topology_destroy(topo);
		processChildNode(*_root);
		accumulateCores(*_root);
	}

	void SystemTopology::accumulateCores(const ProcessorNode& node)
	{
		for (const auto& child : node.getMemoryChildren())
		{
			processChildNode(child);
			accumulateCores(child);
		}

		for (const auto& child : node.getChildren())
		{
			processChildNode(child);
			accumulateCores(child);
		}
	}

	void SystemTopology::processChildNode(const ProcessorNode& node)
	{
		switch (node.getType())
		{
		case ProcessorNodeType::ExecutionUnit:
			_totalLogicalCores++;
			_allProcessors.push_back(&node);
			break;
		case ProcessorNodeType::Core:
			_totalPhysicalCores++;
			break;
		case ProcessorNodeType::NumaNode:
			_totalNumaNodes++;
			break;
		case ProcessorNodeType::Package:
			_totalPackages++;
			break;
		default:
			break;
		}
	}

} /* namespace Elpida */

