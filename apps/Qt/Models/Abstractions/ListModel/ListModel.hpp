//
// Created by klapeto on 21/4/20.
//

#ifndef APPS_QT_MODELS_ABSTRACTIONS_LISTMODEL_HPP
#define APPS_QT_MODELS_ABSTRACTIONS_LISTMODEL_HPP

#include "Models/Abstractions/CollectionModel.hpp"
#include "ListModelItem.hpp"

namespace Elpida {

	template<typename T>
	class ListModel: public CollectionModel<T>
	{
	public:

		template<typename TR>
		void add(TR&& item)
		{
			auto itr = _items.insert(_items.end(), ListModelItem<T>(*this, std::forward<TR>(item)));
			auto& insItem = _items.back();

			insItem._iterator = itr;
			CollectionModel<T>::onDataChanged();
			CollectionModel<T>::onItemAdded(insItem);
		}

		void remove(ListModelItem<T>& item)
		{
			_items.erase(item._iterator);
			CollectionModel<T>::onDataChanged();
			CollectionModel<T>::onItemRemoved(item);
		}

		void clear()
		{
			_items.clear();
			CollectionModel<T>::onCleared();
		}

		const std::list<ListModelItem<T>>& getItems() const
		{
			return _items;
		}

		~ListModel() override = default;
	private:
		std::list<ListModelItem<T>> _items;
	};
}

#endif //APPS_QT_MODELS_ABSTRACTIONS_LISTMODEL_HPP
