/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2020  Ioannis Panagiotopoulos
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

//
// Created by klapeto on 21/4/20.
//

#ifndef APPS_QT_MODELS_ABSTRACTIONS_LISTMODEL_HPP
#define APPS_QT_MODELS_ABSTRACTIONS_LISTMODEL_HPP

#include "Models/Abstractions/CollectionModel.hpp"
#include "Models/Abstractions/ListModel/ListModelItem.hpp"

#include <cstdlib>

namespace Elpida
{

	template<typename T>
	class ListModel : public CollectionModel<T>
	{
	 public:

		template<typename TR>
		const T& Add(TR&& item)
		{
			auto itr = _items.insert(_items.end(), ListModelItem<T>(*this, std::forward<TR>(item)));
			auto& insItem = _items.back();

			insItem._iterator = itr;
			CollectionModel<T>::OnDataChanged();
			CollectionModel<T>::OnItemAdded(insItem);
			return insItem.GetValue();
		}

		void Remove(ListModelItem<T>& item)
		{
			_items.erase(item._iterator);
			CollectionModel<T>::OnDataChanged();
			CollectionModel<T>::OnItemRemoved(item);
		}

		void Clear()
		{
			_items.clear();
			CollectionModel<T>::sOnCleared();
		}

		[[nodiscard]]
		std::size_t Size() const
		{
			return _items.size();
		}

		[[nodiscard]]
		bool Empty() const
		{
			return _items.empty();
		}

		const std::list<ListModelItem<T>>& GetItems() const
		{
			return _items;
		}

		~ListModel() override = default;
	 private:
		std::list<ListModelItem<T>> _items;
	};
}

#endif //APPS_QT_MODELS_ABSTRACTIONS_LISTMODEL_HPP
