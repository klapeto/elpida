/*
 * TaskTopology.cpp
 *
 *  Created on: 16 Νοε 2019
 *      Author: klapeto
 */

#include "Elpida/TaskAffinity.hpp"

#include "Elpida/Topology/SystemTopology.hpp"
#include "Elpida/Topology/ProcessorNode.hpp"

namespace Elpida
{

	static const ProcessorNode* findNode(const Array<ProcessorNode>& nodes, int processor)
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

