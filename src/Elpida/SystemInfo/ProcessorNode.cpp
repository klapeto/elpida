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
 * ProcessorNode.cpp
 *
 *  Created on: 12 Ιαν 2019
 *      Author: klapeto
 */

#include "Elpida/SystemInfo/ProcessorNode.hpp"

#include "hwloc.h"
#include "Elpida/ElpidaException.hpp"
#include "Elpida/Config.hpp"

#ifdef ELPIDA_LINUX
#include <numa.h>
#else
#include <windows.h>
#endif

namespace Elpida
{

	ProcessorNode::ProcessorNode(ProcessorNode* parent, const std::vector<CpuKind>& cpuKinds, void* rootObj, void* node)
		: _value(0), _type(Type::Unknown), _osIndex(0)
	{
		switch (((hwloc_obj_t)node)->type)
		{
		case HWLOC_OBJ_MACHINE:
			loadMachine(node);
			break;
		case HWLOC_OBJ_CORE:
			loadCore(node);
			break;
		case HWLOC_OBJ_L1CACHE:
		case HWLOC_OBJ_L1ICACHE:
		case HWLOC_OBJ_L2CACHE:
		case HWLOC_OBJ_L2ICACHE:
		case HWLOC_OBJ_L3CACHE:
		case HWLOC_OBJ_L3ICACHE:
		case HWLOC_OBJ_L4CACHE:
		case HWLOC_OBJ_L5CACHE:
			loadCache(node);
			break;
		case HWLOC_OBJ_GROUP:
			loadGroup(node);
			break;
		case HWLOC_OBJ_NUMANODE:
			loadNumaNode(node);
			break;
		case HWLOC_OBJ_PACKAGE:
			loadPackage(node);
			break;
		case HWLOC_OBJ_PU:
		{
			loadExecutionUnit(node);

			auto kindIndex = hwloc_cpukinds_get_by_cpuset((hwloc_topology_t)rootObj, ((hwloc_obj_t)node)->cpuset, 0);

			if (kindIndex != -1)
			{
				_cpuKind = cpuKinds.at(kindIndex);
			}
		}
			break;
		default:
			break;
		}

		if (parent)
		{
			_parent = *parent;
		}

		loadChildren(cpuKinds, rootObj, node);
	}

	void ProcessorNode::loadMachine(void* node)
	{
		_type = Type::Machine;
		_osIndex = ((hwloc_obj_t)node)->os_index;
		_name = "Machine";
	}

	void ProcessorNode::loadPackage(void* node)
	{
		_type = Type::Package;
		_osIndex = ((hwloc_obj_t)node)->os_index;
		_name = "Package";
	}

	void ProcessorNode::loadNumaNode(void* node)
	{
		_type = Type::NumaNode;
		_value = ((hwloc_obj_t)node)->attr->numanode.local_memory;
		_osIndex = ((hwloc_obj_t)node)->os_index;
		_name = "Numa Node";
	}

	void ProcessorNode::loadGroup(void* node)
	{
		_type = Type::Group;
		_name = "Group";
	}

	void ProcessorNode::loadCore(void* node)
	{
		_type = Type::Core;
		_osIndex = ((hwloc_obj_t)node)->os_index;
		_name = "Core";
	}

	void ProcessorNode::loadCache(void* node)
	{
		switch (((hwloc_obj_t)node)->type)
		{
		case HWLOC_OBJ_L1CACHE:
			_type = Type::L1DCache;
			_name = "L1D";
			break;
		case HWLOC_OBJ_L1ICACHE:
			_type = Type::L1ICache;
			_name = "L1I";
			break;
		case HWLOC_OBJ_L2CACHE:
			_type = Type::L2DCache;
			_name = "L2D";
			break;
		case HWLOC_OBJ_L2ICACHE:
			_type = Type::L2ICache;
			_name = "L2I";
			break;
		case HWLOC_OBJ_L3CACHE:
			_type = Type::L3DCache;
			_name = "L3D";
			break;
		case HWLOC_OBJ_L3ICACHE:
			_type = Type::L3ICache;
			_name = "L3I";
			break;
		case HWLOC_OBJ_L4CACHE:
			_type = Type::L4Cache;
			_name = "L4";
			break;
		case HWLOC_OBJ_L5CACHE:
			_type = Type::L5Cache;
			_name = "L5";
			break;
		default:
			_type = Type::Unknown;
			_name = "Unknown Cache";
			break;
		}

		_value = ((hwloc_obj_t)node)->attr->cache.size;
	}

	void ProcessorNode::loadExecutionUnit(void* node)
	{
		_type = Type::ExecutionUnit;
		_osIndex = ((hwloc_obj_t)node)->os_index;
		_name = "EU";
	}

	void ProcessorNode::loadChildren(const std::vector<CpuKind>& cpuKinds, void* rootObj, void* node)
	{
		auto memChild = ((hwloc_obj_t)node)->memory_first_child;
		auto memoryArity = ((hwloc_obj_t)node)->memory_arity;
		_memoryChildren.reserve(memoryArity);
		for (auto i = 0u; i < memoryArity; ++i)
		{
			if (memChild != nullptr)
			{
				_memoryChildren.push_back(ProcessorNode(this, cpuKinds, rootObj, memChild));
			}
			memChild = memChild->next_sibling;
		}

		auto childrenArity = ((hwloc_obj_t)node)->arity;
        _children.reserve(memoryArity);
		for (auto i = 0u; i < childrenArity; ++i)
		{
			_children.push_back(ProcessorNode(this, cpuKinds, rootObj, ((hwloc_obj_t)node)->children[i]));
		}
	}

	void ProcessorNode::loadSiblings()
	{
		for (auto& child : _children)
		{
			for (auto& child2 : _children)
			{
				if (&child != &child2)
				{
					child.addSibling(child2);
				}
			}
			child.loadSiblings();
		}
	}

	bool ProcessorNode::isOsIndexValid() const
	{
		return _osIndex != HWLOC_UNKNOWN_INDEX;
	}

	int ProcessorNode::getNumaNodeId() const
	{
		if (_type == Type::ExecutionUnit)
		{
#ifdef ELPIDA_LINUX
			return numa_node_of_cpu((int)_osIndex);
#else
			UCHAR NodeNumber;

			GetNumaProcessorNode(_osIndex, &NodeNumber);
			return NodeNumber;
#endif
		}
		else
		{
			throw ElpidaException(FUNCTION_NAME, "Cannot get Numa Node id from a non EU type processor node!");
		}
	}

} /* namespace Elpida */

