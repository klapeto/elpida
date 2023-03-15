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

#include "Elpida/Core/Config.hpp"
#include "Models/Abstractions/CollectionModel.hpp"
#include "Models/Abstractions/AssociativeModel/AssociativeItem.hpp"
#include <unordered_map>

namespace Elpida
{
	template<typename TKey, typename TValue>
	class AssociativeModel : public CollectionModel<std::pair<const TKey&, const TValue&>>
	{
	 public:
		using Pair = std::pair<const TKey&, const TValue&>;

		template<typename T>
		void Add(const TKey& key, T&& value)
		{
			auto itr = _values.insert_or_assign(key, std::forward<T>(value));
			const auto& newValue = _values.at(key);
			auto colItem = AssociativeItem<TKey, TValue>(*this, std::pair<const TKey&, const TValue&>(key, newValue));
			CollectionModel<Pair>::OnItemAdded(colItem);
			CollectionModel<Pair>::OnDataChanged();
		}

		[[nodiscard]]
		const TValue& GetConst(const TKey& key) const
		{
			auto itr = _values.find(key);
			if (itr != _values.end())
			{
				return itr->second;
			}
			else
			{
				throw ElpidaException("Key not found on model");
			}
		}

		[[nodiscard]]
		TValue& Get(const TKey& key) const
		{
			auto itr = _values.find(key);
			if (itr != _values.end())
			{
				return const_cast<TValue&>(itr->second);
			}
			else
			{
				throw ElpidaException("Key not found on model");
			}
		}

		[[nodiscard]]
		bool ContainsKey(const TKey& key) const
		{
			return _values.find(key) != _values.end();
		}

		void Remove(const TKey& key)
		{
			auto itr = _values.find(key);
			if (itr != _values.end())
			{
				auto value = itr->second;
				_values.erase(key);
				auto colItem = AssociativeItem<TKey, TValue>(*this, std::pair<const TKey&, const TValue&>(key, value));
				CollectionModel<Pair>::OnItemRemoved(colItem);
				CollectionModel<Pair>::OnDataChanged();
			}
		}

		void Clear()
		{
			_values.clear();
			CollectionModel<Pair>::OnCleared();
		}

		[[nodiscard]]
		bool Empty() const noexcept
		{
			return _values.empty();
		}

		[[nodiscard]]
		size_t Size() const noexcept
		{
			return _values.size();
		}

		auto begin() noexcept
		{
			return _values.begin();
		}

		auto begin() const noexcept
		{
			return _values.begin();
		}

		auto cbegin() const noexcept
		{
			return _values.begin();
		}

		auto end() noexcept
		{
			return _values.end();
		}

		auto end() const noexcept
		{
			return _values.end();
		}

		auto cend() const noexcept
		{
			return _values.end();
		}

		AssociativeModel() = default;
		~AssociativeModel() override = default;
	 private:
		std::unordered_map<TKey, TValue> _values;
	};
}

#endif //APPS_QT_MODELS_ABSTRACTIONS_ASSOCIATIVEMODEL_HPP
