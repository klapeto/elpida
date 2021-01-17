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

#ifndef APPS_QT_MODELS_ABSTRACTIONS_EVENTARGS_COLLECTIONCHANGEDEVENTARGS_HPP
#define APPS_QT_MODELS_ABSTRACTIONS_EVENTARGS_COLLECTIONCHANGEDEVENTARGS_HPP

#include "Models/Abstractions/CollectionItem.hpp"

namespace Elpida
{
	template<typename T>
	class CollectionChangedEventArgs
	{
	public:

		const CollectionItem<T>& getItem() const
		{
			return _item;
		}

		explicit CollectionChangedEventArgs(const CollectionItem<T>& item)
			: _item(item)
		{
		}
	private:
		const CollectionItem<T>& _item;
	};
}

#endif //APPS_QT_MODELS_ABSTRACTIONS_EVENTARGS_COLLECTIONCHANGEDEVENTARGS_HPP
