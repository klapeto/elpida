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
// Created by klapeto on 13/4/20.
//

#ifndef APPS_QT_MODELS_ABSTRACTIONS_COLLECTIONMODEL_HPP
#define APPS_QT_MODELS_ABSTRACTIONS_COLLECTIONMODEL_HPP

#include "Models/Abstractions/Model.hpp"
#include "Models/Abstractions/CollectionItem.hpp"

namespace Elpida::Application
{
	template<typename T>
	class CollectionModel : public Model
	{
	 public:
		Event<const CollectionItem<T>&>& ItemAdded() const
		{
			return _itemAdded;
		}

		Event<const CollectionItem<T>&>& ItemRemoved() const
		{
			return _itemRemoved;
		}

		Event<>& Cleared() const
		{
			return _cleared;
		}

		CollectionModel() = default;
		CollectionModel(const CollectionModel<T>&) = delete;
		CollectionModel(CollectionModel<T>&&) noexcept = delete;
		CollectionModel<T>& operator=(const CollectionModel<T>&) = delete;
		CollectionModel<T>& operator=(CollectionModel<T>&&) noexcept = delete;
		~CollectionModel() override = default;
	 private:
		mutable Event<const CollectionItem<T>&> _itemAdded;
		mutable Event<const CollectionItem<T>&> _itemRemoved;
		mutable Event<> _cleared;
	 protected:
		void OnItemAdded(const CollectionItem<T>& item)
		{
			_itemAdded.Raise(item);
		}

		void OnItemRemoved(const CollectionItem<T>& item)
		{
			_itemRemoved.Raise(item);
		}

		void OnCleared()
		{
			_cleared.Raise();
		}
	};
}

#endif //APPS_QT_MODELS_ABSTRACTIONS_COLLECTIONMODEL_HPP