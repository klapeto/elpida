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
 * ProcessorNode.cpp
 *
 *  Created on: 12 Ιαν 2019
 *      Author: klapeto
 */

#include "Elpida/Topology/ProcessorNode.hpp"
#include <hwloc.h>

namespace Elpida
{

	static constexpr const char* UnknownOsIndexStr = "[No Index]";

	ProcessorNode::ProcessorNode(void* node)
			: _type(Type::Unknown), _value(0)
	{
		switch (((hwloc_obj_t) node)->type)
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
				loadExecutionUnit(node);
				break;
			default:
				break;
		}

		loadChildren(node);
	}

	void ProcessorNode::loadMachine(void* node)
	{
		_type = Type::Machine;
		_name = "Machine: "
		        + ((((hwloc_obj_t) node)->os_index != HWLOC_UNKNOWN_INDEX ) ?
		                std::to_string(((hwloc_obj_t) node)->os_index) : String(UnknownOsIndexStr));
	}

	void ProcessorNode::loadPackage(void* node)
	{
		_type = Type::Package;
		_name = "Package: "
		        + ((((hwloc_obj_t) node)->os_index != HWLOC_UNKNOWN_INDEX ) ?
		                std::to_string(((hwloc_obj_t) node)->os_index) : String(UnknownOsIndexStr));
	}

	void ProcessorNode::loadNumaNode(void* node)
	{
		_type = Type::NumaNode;
		_value = ((hwloc_obj_t) node)->attr->numanode.local_memory;
		_name = "Numa Node: "
		        + ((((hwloc_obj_t) node)->os_index != HWLOC_UNKNOWN_INDEX ) ?
		                std::to_string(((hwloc_obj_t) node)->os_index) : String(UnknownOsIndexStr));
	}

	void ProcessorNode::loadGroup(void* node)
	{
		_type = Type::Group;
		_name = "Group";
	}

	void ProcessorNode::loadCore(void* node)
	{
		_type = Type::Core;
		_name = "Core: "
		        + ((((hwloc_obj_t) node)->os_index != HWLOC_UNKNOWN_INDEX ) ?
		                std::to_string(((hwloc_obj_t) node)->os_index) : String(UnknownOsIndexStr));
	}

	void ProcessorNode::loadCache(void* node)
	{
		switch (((hwloc_obj_t) node)->type)
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

		_value = ((hwloc_obj_t) node)->attr->cache.size;
	}

	void ProcessorNode::loadExecutionUnit(void* node)
	{
		_type = Type::ExecutionUnit;
		_name = "EU: "
		        + ((((hwloc_obj_t) node)->os_index != HWLOC_UNKNOWN_INDEX ) ?
		                std::to_string(((hwloc_obj_t) node)->os_index) : String(UnknownOsIndexStr));
	}

	void ProcessorNode::loadChildren(void* node)
	{
		auto memChild = ((hwloc_obj_t) node)->memory_first_child;
		for (auto i = 0u; i < ((hwloc_obj_t) node)->memory_arity; ++i)
		{
			if (memChild != nullptr)
			{
				_children.push_back(ProcessorNode(memChild));
			}
			memChild = memChild->next_sibling;
		}

		for (auto i = 0u; i < ((hwloc_obj_t) node)->arity; ++i)
		{
			_children.push_back(ProcessorNode(((hwloc_obj_t) node)->children[i]));
		}
	}

} /* namespace Elpida */

