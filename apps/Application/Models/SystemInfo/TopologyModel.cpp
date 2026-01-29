/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

//
// Created by klapeto on 20/3/2023.
//

#include "TopologyModel.hpp"

namespace Elpida::Application
{
	TopologyModel::TopologyModel(TopologyNodeModel root, unsigned int fastestProcessor)
			:_root(std::move(root)), _fastestProcessor(fastestProcessor), _totalPackages(0), _totalNumaNodes(0), _totalPhysicalCores(0), _totalLogicalCores(0)
	{
		_rootDataChanged = _root.DataChanged().Subscribe([this]()
		{
			SetSelectedLeafNodes();
			OnDataChanged();
		});
		CountNode(_root);
	}

	TopologyNodeModel& TopologyModel::GetRoot()
	{
		return _root;
	}

	static void
	GetSelectedNodes(std::vector<std::reference_wrapper<const TopologyNodeModel>>& accumulator,
			const TopologyNodeModel& node)
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
	GetLeafNodesImpl(std::vector<std::reference_wrapper<const TopologyNodeModel>>& accumulator,
			const TopologyNodeModel& node)
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

	TopologyModel::TopologyModel(TopologyModel&& other) noexcept
			:_root(std::move(other._root)), _selectedLeafNodes(std::move(other._selectedLeafNodes)),
			 _leafNodes(std::move(other._leafNodes)), _fastestProcessor(other._fastestProcessor)
	{
		_rootDataChanged = _root.DataChanged().Subscribe([this]()
		{
			SetSelectedLeafNodes();
			OnDataChanged();
		});
	}

	TopologyModel& TopologyModel::operator=(TopologyModel&& other) noexcept
	{
		_root = std::move(other._root);
		_selectedLeafNodes = std::move(other._selectedLeafNodes);
		_leafNodes = std::move(other._leafNodes);
		_fastestProcessor = other._fastestProcessor;
		_totalPackages = other._totalPackages;
		_totalNumaNodes = other._totalNumaNodes;
		_totalPhysicalCores = other._totalPhysicalCores;
		_totalLogicalCores = other._totalLogicalCores;
		_rootDataChanged = _root.DataChanged().Subscribe([this]()
		{
			SetSelectedLeafNodes();
			OnDataChanged();
		});
		return *this;
	}

	const TopologyNodeModel& TopologyModel::GetRoot() const
	{
		return _root;
	}

	unsigned int TopologyModel::GetFastestProcessor() const
	{
		return _fastestProcessor;
	}

	void TopologyModel::CountNode(const TopologyNodeModel& node)
	{
		switch (node.GetType())
		{
		case TopologyNodeType::ProcessingUnit:
			_totalLogicalCores++;
			break;
		case TopologyNodeType::Core:
			_totalPhysicalCores++;
			break;
		case TopologyNodeType::NumaDomain:
			_totalNumaNodes++;
			break;
		case TopologyNodeType::Package:
			_totalPackages++;
			break;
		default:
			break;
		}

		for (auto& child : node.GetMemoryChildren())
		{
			CountNode(child);
		}

		for (auto& child : node.GetChildren())
		{
			CountNode(child);
		}
	}

	size_t TopologyModel::GetTotalLogicalCores() const
	{
		return _totalLogicalCores;
	}

	size_t TopologyModel::GetTotalPhysicalCores() const
	{
		return _totalPhysicalCores;
	}

	size_t TopologyModel::GetTotalNumaNodes() const
	{
		return _totalNumaNodes;
	}

	size_t TopologyModel::GetTotalPackages() const
	{
		return _totalPackages;
	}
} // Application