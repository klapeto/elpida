//
// Created by klapeto on 13/4/20.
//

#ifndef APPS_QT_MODELS_ABSTRACTIONS_COLLECTIONMODEL_HPP
#define APPS_QT_MODELS_ABSTRACTIONS_COLLECTIONMODEL_HPP

#include "Models/Abstractions/EventArgs/CollectionChangedEventArgs.hpp"
#include "Model.hpp"
#include "CollectionItem.hpp"
#include <type_traits>

namespace Elpida
{

	template<typename T>
	class CollectionModel : public Model
	{
	public:

		Event<const CollectionChangedEventArgs<T>&> itemAdded;
		Event<const CollectionChangedEventArgs<T>&> itemRemoved;
		Event<> cleared;

		template<typename TR>
		void add(TR&& item)
		{
			auto itr = _items.insert(_items.end(), CollectionItem<T>(*this, std::forward<TR>(item)));
			auto& insItem = _items.back();

			insItem._iterator = itr;
			onDataChanged();
			onItemAdded(insItem);
		}

		void remove(CollectionItem<T>& item)
		{
			_items.erase(item._iterator);
			onDataChanged();
			onItemRemoved(item);
		}

		void clear()
		{
			_items.clear();
			onCleared();
		}

		const std::list<CollectionItem<T>>& getItems() const
		{
			return _items;
		}

		CollectionModel() = default;
		CollectionModel(const CollectionModel<T>&) = delete;
		CollectionModel(CollectionModel<T>&&) noexcept = delete;
		CollectionModel<T>& operator=(const CollectionModel<T>&) = delete;
		CollectionModel<T>& operator=(CollectionModel<T>&&) noexcept = delete;
		~CollectionModel() override = default;
	private:
		std::list<CollectionItem<T>> _items;

		void onItemAdded(const CollectionItem<T>& item)
		{
			auto args = CollectionChangedEventArgs<T>(item);
			itemAdded.raise(args);
		}

		void onItemRemoved(const CollectionItem<T>& item)
		{
			auto args = CollectionChangedEventArgs<T>(item);
			itemRemoved.raise(args);
		}

		void onCleared()
		{
			cleared.raise();
		}
	};
}

#endif //APPS_QT_MODELS_ABSTRACTIONS_COLLECTIONMODEL_HPP