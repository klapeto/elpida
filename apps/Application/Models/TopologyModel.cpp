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
	GetSelectedNodes(std::vector<std::reference_wrapper<TopologyNodeModel>>& accumulator, TopologyNodeModel& node)
	{
		if (node.IsSelected() && node.GetType() == TopologyNodeType::ProcessingUnit)
		{
			accumulator.emplace_back(node);
		}

		for (auto& child: node.GetMemoryChildren())
		{
			GetSelectedNodes(accumulator, child);
		}

		for (auto& child: node.GetChildren())
		{
			GetSelectedNodes(accumulator, child);
		}
	}

	void TopologyModel::SetSelectedLeafNodes()
	{
		_selectedLeafNodes.clear();
		GetSelectedNodes(_selectedLeafNodes, _root);
	}

	const std::vector<std::reference_wrapper<TopologyNodeModel>>& TopologyModel::GetSelectedLeafNodes()
	{
		return _selectedLeafNodes;
	}
} // Application