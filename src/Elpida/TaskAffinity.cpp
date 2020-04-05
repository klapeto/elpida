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
 * TaskTopology.cpp
 *
 *  Created on: 16 Νοε 2019
 *      Author: klapeto
 */

#include "Elpida/TaskAffinity.hpp"

#include <initializer_list>

#include "Elpida/Topology/ProcessorNode.hpp"
#include "Elpida/Topology/SystemTopology.hpp"

namespace Elpida
{

	static const ProcessorNode* findNode(const std::vector<ProcessorNode>& nodes, int processor)
	{
		for (const auto& node : nodes)
		{
			if (node.getType() != ProcessorNode::Type::ExecutionUnit)
			{
				auto childNode = findNode(node.getChildren(), processor);
				if (childNode != nullptr)
				{
					return childNode;
				}
			}
			else if (node.getOsIndex() == processor)
			{
				return &node;
			}
		}
		return nullptr;
	}

	TaskAffinity::TaskAffinity(std::initializer_list<int> processors)
	{
		const auto& topology = SystemTopology::getTopology();

		for (int i : processors)
		{
			auto node = findNode(topology.getRoot()->getChildren(), i);
			if (node != nullptr)
			{
				_nodes.push_back(node);
			}
		}
	}

} /* namespace Elpida */

