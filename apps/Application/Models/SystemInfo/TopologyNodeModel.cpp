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
			std::optional<int> efficiency,
			std::vector<TopologyNodeModel>&& children,
			std::vector<TopologyNodeModel>&& memoryChildren)
			:_children(std::move(children)),
			 _memoryChildren(std::move(memoryChildren)),
			 _type(type),
			 _osIndex(osIndex),
			 _size(size),
			 _efficiency(efficiency),
			 _selected(false)
	{

	}

	void TopologyNodeModel::SetSelected(bool selected)
	{
		Transactional<TopologyNodeModel>([selected](TopologyNodeModel& model)
		{
			model.SetSelectedInternal(selected);
		});
	}

	const std::vector<TopologyNodeModel>& TopologyNodeModel::GetChildren() const
	{
		return _children;
	}

	const std::vector<TopologyNodeModel>& TopologyNodeModel::GetMemoryChildren() const
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
		if (_parent.has_value())
		{
			_parent.value().get().OnDataChanged();
		}
	}

	static std::optional<std::reference_wrapper<TopologyNodeModel>> GetLastCacheNode(TopologyNodeModel& topologyNode)
	{
		std::optional<std::reference_wrapper<TopologyNodeModel>> lastNode;
		std::optional<std::reference_wrapper<TopologyNodeModel>> currentNode = topologyNode;
		while (currentNode.has_value())
		{
			switch (currentNode->get().GetType())
			{
			case TopologyNodeType::L1ICache:
				case TopologyNodeType::L1DCache:
				case TopologyNodeType::L2ICache:
				case TopologyNodeType::L2DCache:
				case TopologyNodeType::L3ICache:
				case TopologyNodeType::L3DCache:
				case TopologyNodeType::L4Cache:
				case TopologyNodeType::L5Cache:
					lastNode = currentNode;
					break;
			}
			currentNode = currentNode->get().GetParent();
		}

		return lastNode;
	}

	void TopologyNodeModel::SetRelations()
	{
		for (auto& child : _children)
		{
			child._parent = *this;
			child.SetRelations();
		}

		for (auto& child : _memoryChildren)
		{
			child._parent = *this;
			child.SetRelations();
		}

		if (_type == TopologyNodeType::ProcessingUnit)
		{
			_lastCache = GetLastCacheNode(*this);
		}
	}

	void TopologyNodeModel::SetSelectedInternal(bool selected)
	{
		_selected = selected;
		for (auto& child : _children)
		{
			child.SetSelectedInternal(selected);
		}

		for (auto& child : _memoryChildren)
		{
			child.SetSelectedInternal(selected);
		}
		Model::OnDataChanged();
	}

	std::optional<int> TopologyNodeModel::GetEfficiency() const
	{
		return _efficiency;
	}

	std::vector<TopologyNodeModel>& TopologyNodeModel::GetMemoryChildren()
	{
		return _memoryChildren;
	}

	std::vector<TopologyNodeModel>& TopologyNodeModel::GetChildren()
	{
		return _children;
	}

	const std::optional<std::reference_wrapper<TopologyNodeModel>>& TopologyNodeModel::GetLastCache() const
	{
		return _lastCache;
	}

	const std::optional<std::reference_wrapper<TopologyNodeModel>>& TopologyNodeModel::GetParent() const
	{
		return _parent;
	}
} // Elpida::Application