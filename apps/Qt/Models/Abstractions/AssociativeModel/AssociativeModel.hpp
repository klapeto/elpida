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

#ifndef APPS_QT_MODELS_ABSTRACTIONS_ASSOCIATIVEMODEL_HPP
#define APPS_QT_MODELS_ABSTRACTIONS_ASSOCIATIVEMODEL_HPP

#include "Models/Abstractions/CollectionModel.hpp"
#include "Models/Abstractions/AssociativeModel/AssociativeItem.hpp"
#include <unordered_map>
#include <Elpida/Config.hpp>

namespace Elpida
{
	template<typename TKey, typename TValue>
	class AssociativeModel : public CollectionModel<std::pair<const TKey&, const TValue&>>
	{
	public:
		using Pair =std::pair<const TKey&, const TValue&>;

		template<typename T>
		void add(const TKey& key, T&& value)
		{
			auto itr = _values.insert_or_assign(key, std::forward<T>(value));
			const auto& newValue = _values.at(key);
			auto colItem = AssociativeItem<TKey,TValue>(*this, std::pair<const TKey&, const TValue&>(key, newValue));
			CollectionModel<Pair>::onItemAdded(colItem);
			CollectionModel<Pair>::onDataChanged();
		}

		const TValue& getConst(const TKey& key) const
		{
			auto itr = _values.find(key);
			if (itr != _values.end())
			{
				return itr->second;
			}
			else
			{
				throw ElpidaException(FUNCTION_NAME, "Key not found on model");
			}
		}

		TValue& get(const TKey& key) const
		{
			auto itr = _values.find(key);
			if (itr != _values.end())
			{
				return const_cast<TValue&>(itr->second);
			}
			else
			{
				throw ElpidaException(FUNCTION_NAME, "Key not found on model");
			}
		}

		void remove(const TKey& key)
		{
			auto itr = _values.find(key);
			if (itr != _values.end())
			{
				auto value = itr->second;
				_values.erase(key);
				CollectionModel<std::pair<TKey, TValue>>
				::onItemRemoved(CollectionItem<std::pair<TKey, TValue>>(std::make_pair<TKey, TValue>(key, value)));
				CollectionModel<std::pair<TKey, TValue>>::dataChanged();
			}
		}

		void clear() 
		{
			_values.clear();
			CollectionModel<Pair>::onCleared();
		}

		AssociativeModel() = default;
		~AssociativeModel() override = default;
	private:
		std::unordered_map<TKey, TValue> _values;
	};
}


#endif //APPS_QT_MODELS_ABSTRACTIONS_ASSOCIATIVEMODEL_HPP
