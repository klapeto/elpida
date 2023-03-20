//
// Created by klapeto on 20/3/2023.
//

#include "TopologyModel.hpp"

namespace Elpida::Application
{
	TopologyModel::TopologyModel(TopologyNodeModel root)
			: _root(std::move(root))
	{

	}

	const TopologyNodeModel& TopologyModel::GetRoot() const
	{
		return _root;
	}
} // Application