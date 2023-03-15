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

#include "Models/Abstractions/EventArgs/CollectionChangedEventArgs.hpp"
#include "Models/Abstractions/Model.hpp"

namespace Elpida::Application
{
	template<typename T>
	class CollectionModel : public Model
	{
	 public:
		Event<const CollectionChangedEventArgs<T>&>& ItemAdded()
		{
			return _itemAdded;
		}

		Event<const CollectionChangedEventArgs<T>&>& ItemRemoved()
		{
			return _itemRemoved;
		}

		Event<const CollectionChangedEventArgs<T>&>& Cleared()
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
		Event<const CollectionChangedEventArgs<T>&> _itemAdded;
		Event<const CollectionChangedEventArgs<T>&> _itemRemoved;
		Event<> _cleared;
	 protected:
		void onItemAdded(const CollectionItem<T>& item)
		{
			auto args = CollectionChangedEventArgs<T>(item);
			_itemAdded.Raise(args);
		}

		void onItemRemoved(const CollectionItem<T>& item)
		{
			auto args = CollectionChangedEventArgs<T>(item);
			_itemRemoved.Raise(args);
		}

		void onCleared()
		{
			_cleared.Raise();
		}
	};
}

#endif //APPS_QT_MODELS_ABSTRACTIONS_COLLECTIONMODEL_HPP