//
// Created by klapeto on 13/4/20.
//

#ifndef APPS_QT_MODELS_ABSTRACTIONS_COLLECTIONITEM_HPP
#define APPS_QT_MODELS_ABSTRACTIONS_COLLECTIONITEM_HPP

#include <list>

namespace Elpida
{
	template<typename TR>
	class CollectionModel;

	template<typename T>
	class CollectionItem
	{
	private:
		using Iterator = typename std::list<CollectionItem<T>>::iterator;

	public:

		const T& getValue() const
		{
			return _value;
		}

	private:
		const CollectionModel<T>& _owner;
		Iterator _iterator;
		T _value;

		template<typename TR>
		explicit CollectionItem(const CollectionModel<T>& owner, TR&& value)
			: _owner(owner), _value(std::forward<TR>(value))
		{

		}

		template <typename TR>
		friend class CollectionModel;
	};
}


#endif //APPS_QT_MODELS_ABSTRACTIONS_COLLECTIONITEM_HPP
