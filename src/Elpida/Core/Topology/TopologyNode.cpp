//
// Created by klapeto on 2/3/2023.
//

#include "Elpida/Core/Topology/TopologyNode.hpp"

namespace Elpida
{
	void TopologyNode::LoadSiblings()
	{
		if (_parent.has_value())
		{
			for (auto& sibling: _parent->get().GetChildren())
			{
				if (sibling.get() != this)
				{
					AddSibling(*sibling);
				}
			}
		}

		for (auto& child: _children)
		{
			child->LoadSiblings();
		}
	}
	void TopologyNode::LoadParents(Optional<Ref<TopologyNode>> parent)
	{
		if (parent.has_value())
		{
			_parent = *parent;
		}
		for (auto& child: _children)
		{
			child->LoadParents(*this);
		}

		for (auto& child: _memoryChildren)
		{
			child->LoadParents(*this);
		}
	}

	void TopologyNode::AddSibling(TopologyNode& node)
	{
		_siblings.emplace_back(node);
	}

	Optional<unsigned int> TopologyNode::GetOsIndex() const
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

	void TopologyNode::SetParent(Ref<const TopologyNode> parent)
	{
		_parent = parent;
	}

	TopologyNode::TopologyNode(NodeType type)
		: _type(type)
	{

	}

	Vector<UniquePtr<TopologyNode>>& TopologyNode::GetMemoryChildren()
	{
		return _memoryChildren;
	}

	Vector<UniquePtr<TopologyNode>>& TopologyNode::GetChildren()
	{
		return _children;
	}

	void TopologyNode::SetOsIndex(unsigned int index)
	{
		_osIndex = index;
	}

	void TopologyNode::PostProcess()
	{
		PostProcessImpl();
		for (auto& child: _children)
		{
			child->PostProcess();
		}

		for (auto& child: _memoryChildren)
		{
			child->PostProcess();
		}
	}

	void TopologyNode::PostProcessImpl()
	{

	}

} // Elpida