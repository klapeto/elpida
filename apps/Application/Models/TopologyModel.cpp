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

	TopologyNodeModel& TopologyModel::GetRoot()
	{
		return _root;
	}
} // Application