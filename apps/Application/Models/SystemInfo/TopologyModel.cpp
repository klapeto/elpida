//
// Created by klapeto on 20/3/2023.
//

#include "TopologyModel.hpp"

namespace Elpida::Application
{
	TopologyModel::TopologyModel(TopologyNodeModel root)
		: _root(std::move(root))
	{
		_rootDataChanged = _root.DataChanged().Subscribe([this]()
		{
		  SetSelectedLeafNodes();
		  OnDataChanged();
		});
	}

	TopologyNodeModel& TopologyModel::GetRoot()
	{
		return _root;
	}

	static void
	GetSelectedNodes(std::vector<std::reference_wrapper<const TopologyNodeModel>>& accumulator, const TopologyNodeModel& node)
	{
		if (node.IsSelected() && node.GetType() == TopologyNodeType::ProcessingUnit)
		{
			accumulator.emplace_back(node);
		}

		for (auto& child : node.GetMemoryChildren())
		{
			GetSelectedNodes(accumulator, child);
		}

		for (auto& child : node.GetChildren())
		{
			GetSelectedNodes(accumulator, child);
		}
	}

	void TopologyModel::SetSelectedLeafNodes()
	{
		_selectedLeafNodes.clear();
		auto& leafs = GetLeafNodes();
		for (auto leaf : leafs)
		{
			if (leaf.get().IsSelected())
			{
				_selectedLeafNodes.push_back(leaf);
			}
		}
		//GetSelectedNodes(_selectedLeafNodes, _root);
	}

	const std::vector<std::reference_wrapper<const TopologyNodeModel>>& TopologyModel::GetSelectedLeafNodes() const
	{
		return _selectedLeafNodes;
	}

	static void
	GetLeafNodesImpl(std::vector<std::reference_wrapper<const TopologyNodeModel>>& accumulator, const TopologyNodeModel& node)
	{
		if (node.GetType() == TopologyNodeType::ProcessingUnit)
		{
			accumulator.emplace_back(node);
		}

		for (auto& child : node.GetMemoryChildren())
		{
			GetLeafNodesImpl(accumulator, child);
		}

		for (auto& child : node.GetChildren())
		{
			GetLeafNodesImpl(accumulator, child);
		}
	}

	const std::vector<std::reference_wrapper<const TopologyNodeModel>>& TopologyModel::GetLeafNodes() const
	{
		if (_leafNodes.empty())
		{
			GetLeafNodesImpl(_leafNodes, _root);
		}
		return _leafNodes;
	}

	const std::vector<std::reference_wrapper<TopologyNodeModel>>& TopologyModel::GetSelectedLeafNodes()
	{
		return reinterpret_cast<const std::vector<std::reference_wrapper<TopologyNodeModel>>&>(_selectedLeafNodes);
	}

	const std::vector<std::reference_wrapper<TopologyNodeModel>>& TopologyModel::GetLeafNodes()
	{
		return reinterpret_cast<const std::vector<std::reference_wrapper<TopologyNodeModel>>&>(reinterpret_cast<const TopologyModel*>(this)->GetLeafNodes());
	}
} // Application