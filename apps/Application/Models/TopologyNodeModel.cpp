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
			  _parent(nullptr),
			  _selected(false)
	{

	}

	void TopologyNodeModel::SetSelected(bool selected)
	{
		Transactional<TopologyNodeModel>([&selected](TopologyNodeModel& model)
		{
			model.SetSelectedInternal(selected);
		});
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

	void TopologyNodeModel::OnDataChanged() const
	{
		Model::OnDataChanged();
		if (_parent != nullptr)
		{
			_parent->OnDataChanged();
		}
	}

	void TopologyNodeModel::SetParents()
	{
		for (auto& child: _children)
		{
			child._parent = this;
			child.SetParents();
		}

		for (auto& child: _memoryChildren)
		{
			child._parent = this;
			child.SetParents();
		}
	}

	void TopologyNodeModel::SetSelectedInternal(bool selected)
	{
		_selected = selected;
		for (auto& child: _children)
		{
			child.SetSelectedInternal(selected);
		}

		for (auto& child: _memoryChildren)
		{
			child.SetSelectedInternal(selected);
		}
		Model::OnDataChanged();
	}
} // Elpida::Application