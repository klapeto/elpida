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
// Created by klapeto on 21/4/20.
//

#ifndef APPS_QT_MODELS_ABSTRACTIONS_LISTMODEL_LISTMODELITEM_HPP
#define APPS_QT_MODELS_ABSTRACTIONS_LISTMODEL_LISTMODELITEM_HPP

#include "Models/Abstractions/CollectionItem.hpp"

#include <list>

namespace Elpida::Application
{
	template<typename TR>
	class ListModel;

	template<typename T>
	class ListModelItem : public CollectionItem<T>
	{
	 private:
		using Iterator = typename std::list<ListModelItem<T>>::iterator;

		template<typename TR>
		explicit ListModelItem(const ListModel<T>& owner, TR&& value)
			: CollectionItem<T>(owner, std::forward<TR>(value))
		{

		}

		Iterator _iterator;

		friend class ListModel<T>;
	};
}

#endif //APPS_QT_MODELS_ABSTRACTIONS_LISTMODEL_LISTMODELITEM_HPP
