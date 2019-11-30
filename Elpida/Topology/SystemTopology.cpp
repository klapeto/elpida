/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2018  Ioannis Panagiotopoulos
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

#include "Elpida/Topology/SystemTopology.hpp"

#include "Elpida/Config.hpp"
#include "Elpida/Topology/ProcessorNode.hpp"
#include <hwloc.h>

#ifdef _elpida_linux
#include <numa.h>
#else
#include <windows.h>
#endif

namespace Elpida
{
	SystemTopology::SystemTopology()
			: _root(nullptr)
	{
		reload();
	}

	SystemTopology::~SystemTopology()
	{
		delete _root;
	}

	void SystemTopology::reload()
	{
		if (_root != nullptr)
		{
			delete _root;
		}
		hwloc_topology_t topo;
		hwloc_topology_init(&topo);
		hwloc_topology_set_all_types_filter(topo, HWLOC_TYPE_FILTER_KEEP_ALL);
		hwloc_topology_load(topo);
		_root = new ProcessorNode(nullptr, hwloc_get_root_obj(topo));
		_root->loadSiblings();	// Now its safe to attempt to recursively load all siblings
		hwloc_topology_destroy(topo);
		accumulateCores(*_root);
	}

	void SystemTopology::accumulateCores(const ProcessorNode& node)
	{
		for (const auto& child : node.getChildren())
		{
			switch (child.getType())
			{
				case ProcessorNode::Type::ExecutionUnit:
					_totalLogicalCores++;
					_allProcessors.push_back(&child);
					break;
				case ProcessorNode::Type::Core:
					_totalPhysicalCores++;
					break;
				default:
					break;
			}
			accumulateCores(child);
		}
	}

	int SystemTopology::getNumaNodeOfProcessor(int processorId)
	{
#if _elpida_linux
		return numa_node_of_cpu(processorId);
#else
		UCHAR NodeNumber;

		GetNumaProcessorNode(i, &NodeNumber);
		return NodeNumber;
#endif
	}

} /* namespace Elpida */

