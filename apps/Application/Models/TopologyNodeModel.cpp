//
// Created by klapeto on 20/3/2023.
//

#include "TopologyNodeModel.hpp"

namespace Elpida::Application
{
	TopologyNodeModel::TopologyNodeModel(
			TopologyNodeType type,
			std::optional<std::size_t> osIndex,
			std::optional<std::size_t> size,
			std::vector<TopologyNodeModel>&& children,
			std::vector<TopologyNodeModel>&& memoryChildren)
			: _children(std::move(children)),
			  _memoryChildren(std::move(memoryChildren)),
			  _type(type),
			  _osIndex(osIndex),
			  _size(size),
			  _selected(false)
	{
	}

	void TopologyNodeModel::SetSelected(bool selected)
	{
		_selected = selected;
		for (auto& child: _children)
		{
			child.SetSelected(selected);
		}

		for (auto& child: _memoryChildren)
		{
			child.SetSelected(selected);
		}

		if (_type == TopologyNodeType::ProcessingUnit)
		{
			OnDataChanged();
		}
	}

	bool TopologyNodeModel::CanBeSelected() const
	{
		return _type == TopologyNodeType::ProcessingUnit;
	}

	std::vector<TopologyNodeModel>& TopologyNodeModel::GetChildren()
	{
		return _children;
	}

	std::vector<TopologyNodeModel>& TopologyNodeModel::GetMemoryChildren()
	{
		return _memoryChildren;
	}

	TopologyNodeType TopologyNodeModel::GetType() const
	{
		return _type;
	}

	std::optional<std::size_t> TopologyNodeModel::GetOsIndex() const
	{
		return _osIndex;
	}

	std::optional<std::size_t> TopologyNodeModel::GetSize() const
	{
		return _size;
	}

	bool TopologyNodeModel::IsSelected() const
	{
		return _selected;
	}
} // Elpida::Application