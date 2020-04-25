//
// Created by klapeto on 22/4/20.
//

#ifndef APPS_QT_MODELS_ABSTRACTIONS_ASSOCIATIVEMODEL_ASSOCIATIVEITEM_HPP
#define APPS_QT_MODELS_ABSTRACTIONS_ASSOCIATIVEMODEL_ASSOCIATIVEITEM_HPP

#include "Models/Abstractions/CollectionItem.hpp"

namespace Elpida
{
	template<typename T, typename R>
	class AssociativeModel;

	template<typename TKey, typename TValue>
	class AssociativeItem : public CollectionItem<std::pair<const TKey&, const TValue&>>
	{
	public:
		AssociativeItem(const AssociativeModel<TKey, TValue>& owner, std::pair<const TKey&, const TValue&>&& pair)
			: CollectionItem<std::pair<const TKey&, const TValue&>>(owner, pair)
		{

		}
		~AssociativeItem() = default;
	};
}


#endif //APPS_QT_MODELS_ABSTRACTIONS_ASSOCIATIVEMODEL_ASSOCIATIVEITEM_HPP
