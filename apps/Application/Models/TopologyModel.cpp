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

	std::vector<std::reference_wrapper<TopologyNodeModel>> TopologyModel::GetSelectedLeafNodes()
	{
		std::vector<std::reference_wrapper<TopologyNodeModel>> returnVector;

		GetSelectedNodes(returnVector, _root);

		return returnVector;
	}
} // Application