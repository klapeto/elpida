//
// Created by klapeto on 21/4/20.
//

#ifndef APPS_QT_MODELS_ABSTRACTIONS_LISTMODEL_LISTMODELITEM_HPP
#define APPS_QT_MODELS_ABSTRACTIONS_LISTMODEL_LISTMODELITEM_HPP

#include "Models/Abstractions/CollectionItem.hpp"

namespace Elpida {
	template<typename TR>
	class ListModel;

	template<typename T>
	class ListModelItem : public CollectionItem<T>
	{
		using Iterator = typename std::list<ListModelItem<T>>::iterator;

		template<typename TR>
		explicit ListModelItem(const ListModel<T>& owner, TR&& value)
			: CollectionItem<T>(owner, std::forward<TR>(value))
		{

		}

		Iterator _iterator;
		template<typename TR>
		friend class ListModel;
	};
}



#endif //APPS_QT_MODELS_ABSTRACTIONS_LISTMODEL_LISTMODELITEM_HPP
