//
// Created by klapeto on 2/3/2023.
//

#include "Elpida/Topology/TopologyNode.hpp"

#include "Elpida/Topology/CpuCacheNode.hpp"
#include "Elpida/Topology/NumaNode.hpp"
#include "Elpida/Topology/ProcessingUnitNode.hpp"

#include <hwloc.h>

namespace Elpida
{
	TopologyNode::TopologyNode(Optional<Ref<TopologyNode>> parent, const Vector<CpuKind>& cpuKinds, void* rootObj, void* node)
		: _parent(parent), _type(NodeType::Unknown)
	{
		auto currentNode = (hwloc_obj_t)node;
		switch (currentNode->type)
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
		case HWLOC_OBJ_DIE:
			loadDie(node);
			break;
		case HWLOC_OBJ_PU:
			loadProcessingUnit(node);
			break;
		default:
			break;
		}

		auto index = currentNode->os_index;
		if (index != HWLOC_UNKNOWN_INDEX)
		{
			_osIndex = index;
		}

		loadChildren(cpuKinds, rootObj, node);
	}

	void TopologyNode::loadMachine(void* node)
	{
		_type = NodeType::Machine;
	}

	void TopologyNode::loadPackage(void* node)
	{
		_type = NodeType::Package;
	}

	void TopologyNode::loadDie(void* node)
	{
		_type = NodeType::Die;
	}

	void TopologyNode::loadNumaNode(void* node)
	{
		_type = NodeType::NumaDomain;
	}

	void TopologyNode::loadGroup(void* node)
	{
		_type = NodeType::Group;
	}
	void TopologyNode::loadCore(void* node)
	{
		_type = NodeType::Core;
	}

	void TopologyNode::loadCache(void* node)
	{
		auto currentNode = (hwloc_obj_t)node;
		switch (currentNode->type)
		{
		case HWLOC_OBJ_L1CACHE:
			_type = NodeType::L1DCache;
			break;
		case HWLOC_OBJ_L1ICACHE:
			_type = NodeType::L1ICache;
			break;
		case HWLOC_OBJ_L2CACHE:
			_type = NodeType::L2DCache;
			break;
		case HWLOC_OBJ_L2ICACHE:
			_type = NodeType::L2ICache;
			break;
		case HWLOC_OBJ_L3CACHE:
			_type = NodeType::L3DCache;
			break;
		case HWLOC_OBJ_L3ICACHE:
			_type = NodeType::L3ICache;
			break;
		case HWLOC_OBJ_L4CACHE:
			_type = NodeType::L4Cache;
			break;
		case HWLOC_OBJ_L5CACHE:
			_type = NodeType::L5Cache;
			break;
		default:
			_type = NodeType::Unknown;
			break;
		}
	}

	void TopologyNode::loadProcessingUnit(void* node)
	{
		_type = NodeType::ProcessingUnit;
	}

	void TopologyNode::loadChildren(const Vector<CpuKind>& cpuKinds, void* rootObj, void* node)
	{
		auto currentNode = (hwloc_obj_t)node;
		_memoryChildren.reserve(currentNode->memory_arity);
		for (auto memChild = currentNode->memory_first_child;
			 memChild;
			 memChild = memChild->next_sibling)
		{
			_memoryChildren.emplace_back(Load(*this, cpuKinds, rootObj, memChild));
		}

		_children.reserve(currentNode->arity);
		for (auto child = currentNode->first_child;
			 child;
			 child = child->next_sibling)
		{
			_children.push_back(Load(*this, cpuKinds, rootObj, child));
		}
	}

	void TopologyNode::loadSiblings()
	{
		for (auto& child: _children)
		{
			for (auto& child2: _children)
			{
				if (child.get() != child2.get())
				{
					child->addSibling(*child2);
				}
			}
			child->loadSiblings();
		}
	}
	void TopologyNode::loadParents(Optional<Ref<TopologyNode>> parent)
	{
		if (parent.has_value())
		{
			_parent = *parent;
		}
		for (auto& child: _children)
		{
			child->loadParents(*this);
		}

		for (auto& child: _memoryChildren)
		{
			child->loadParents(*this);
		}
	}

	void TopologyNode::addSibling(TopologyNode& node)
	{
		_siblings.emplace_back(node);
	}

	UniquePtr<TopologyNode> TopologyNode::Load(Optional<Ref<TopologyNode>> parent, const Vector<CpuKind>& cpuKinds, void* rootObj, void* node)
	{
		auto currentNode = (hwloc_obj_t)node;
		switch (currentNode->type)
		{
		case HWLOC_OBJ_L1CACHE:
		case HWLOC_OBJ_L1ICACHE:
		case HWLOC_OBJ_L2CACHE:
		case HWLOC_OBJ_L2ICACHE:
		case HWLOC_OBJ_L3CACHE:
		case HWLOC_OBJ_L3ICACHE:
		case HWLOC_OBJ_L4CACHE:
		case HWLOC_OBJ_L5CACHE:
			return UniquePtr<TopologyNode>(new CpuCacheNode(parent, cpuKinds, rootObj, node));
		case HWLOC_OBJ_NUMANODE:
			return UniquePtr<TopologyNode>(new NumaNode(parent, cpuKinds, rootObj, node));
		case HWLOC_OBJ_PU:
			return UniquePtr<TopologyNode>(new ProcessingUnitNode(parent, cpuKinds, rootObj, node));
		default:
			return UniquePtr<TopologyNode>(new TopologyNode(parent, cpuKinds, rootObj, node));
		}
	}
	Optional<int> TopologyNode::GetOsIndex() const
	{
		return _osIndex;
	}

	const Vector<UniquePtr<TopologyNode>>& TopologyNode::GetChildren() const
	{
		return _children;
	}
	const Vector<UniquePtr<TopologyNode>>& TopologyNode::GetMemoryChildren() const
	{
		return _memoryChildren;
	}

	const Vector<Ref<const TopologyNode>>& TopologyNode::GetSiblings() const
	{
		return _siblings;
	}

	Optional<Ref<const TopologyNode>> TopologyNode::GetParent() const
	{
		return _parent;
	}

	NodeType TopologyNode::GetType() const
	{
		return _type;
	}

	Optional<Ref<const TopologyNode>> TopologyNode::GetLastAncestor(NodeType nodeTypes) const
	{
		Optional<Ref<const TopologyNode>> lastNode;
		Optional<Ref<const TopologyNode>> currentNode = *this;
		while (currentNode.has_value())
		{
			if (currentNode->get().GetType() & nodeTypes)
			{
				lastNode = currentNode;
			}
			currentNode = currentNode->get().GetParent();
		}

		return lastNode;
	}

} // Elpida