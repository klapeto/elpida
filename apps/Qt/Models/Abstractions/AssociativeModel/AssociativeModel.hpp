//
// Created by klapeto on 21/4/20.
//

#ifndef APPS_QT_MODELS_ABSTRACTIONS_ASSOCIATIVEMODEL_HPP
#define APPS_QT_MODELS_ABSTRACTIONS_ASSOCIATIVEMODEL_HPP

#include "Models/Abstractions/CollectionModel.hpp"
#include <unordered_map>
#include <Elpida/Config.hpp>

namespace Elpida
{
	template<typename TKey, typename TValue>
	class AssociativeModel : public CollectionModel<std::pair<TKey, TValue>>
	{
	public:
		void add(const TKey& key, TValue&& value)
		{
			_values.insert(key, value);
			CollectionModel<std::pair<TKey, TValue>>
			::onItemAdded(CollectionItem<std::pair<TKey, TValue>>(std::make_pair<TKey, TValue>(key, value)));
			CollectionModel<std::pair<TKey, TValue>>::dataChanged();
		}

		const TValue& get(const TKey& key) const
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

		AssociativeModel() = default;
		~AssociativeModel() override = default;
	private:
		std::unordered_map<TKey, TValue> _values;
	};
}


#endif //APPS_QT_MODELS_ABSTRACTIONS_ASSOCIATIVEMODEL_HPP
