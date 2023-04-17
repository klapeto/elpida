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

#ifndef APPS_QT_MODELS_ABSTRACTIONS_COLLECTIONITEM_HPP
#define APPS_QT_MODELS_ABSTRACTIONS_COLLECTIONITEM_HPP

#include <list>

namespace Elpida::Application
{
	template<typename TR>
	class CollectionModel;

	template<typename T>
	class CollectionItem
	{
	 public:
		const T& GetValue() const
		{
			return _value;
		}

		CollectionItem(const CollectionItem<T>&) = default;
		CollectionItem(CollectionItem<T>&&) noexcept = default;
		CollectionItem<T>& operator=(const CollectionItem<T>&) = delete;
		CollectionItem<T>& operator=(CollectionItem<T>&&) noexcept = default;
		~CollectionItem() = default;
	 protected:
		const CollectionModel<T>& _owner;
		T _value;

		template<typename TR>
		explicit CollectionItem(const CollectionModel<T>& owner, TR&& value)
			: _owner(owner), _value(std::forward<TR>(value))
		{

		}

		friend class CollectionModel<T>;
	};
}

#endif //APPS_QT_MODELS_ABSTRACTIONS_COLLECTIONITEM_HPP
