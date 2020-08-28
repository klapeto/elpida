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

#include "Elpida/Topology/SystemTopology.hpp"

#include "Elpida/Config.hpp"
#include "Elpida/Topology/ProcessorNode.hpp"
#include <hwloc.h>

#ifdef ELPIDA_LINUX
#include <sys/resource.h>
#else
#include <windows.h>
#include <iostream>
#endif

namespace Elpida
{
	SystemTopology::SystemTopology()
		: _root(nullptr), _depth(0), _totalLogicalCores(0), _totalPhysicalCores(0)
	{
		reload();
	}

	SystemTopology::~SystemTopology()
	{
		delete _root;
	}

	void SystemTopology::reload()
	{
		delete _root;
		hwloc_topology_t topo;
		hwloc_topology_init(&topo);
		hwloc_topology_set_all_types_filter(topo, HWLOC_TYPE_FILTER_KEEP_ALL);
		hwloc_topology_load(topo);
		_root = new ProcessorNode(nullptr, hwloc_get_root_obj(topo));
		_root->loadSiblings();    // Now its safe to attempt to recursively load all siblings
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

	void SystemTopology::setProcessPriority(SystemTopology::ProcessPriority priority)
	{
#ifdef ELPIDA_LINUX
		switch (priority)
		{
		case ProcessPriority::High:
			setpriority(PRIO_PROCESS, 0, PRIO_MIN);
			break;
		default:
			setpriority(PRIO_PROCESS, 0, 0);
			break;
		}
#else
		if(!SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS))
		{
			std::cout << "Warning! Failed to set process priority: " << GetLastError() << std::endl;
		}
#endif
	}

} /* namespace Elpida */

