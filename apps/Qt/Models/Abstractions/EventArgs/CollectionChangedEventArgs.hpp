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
