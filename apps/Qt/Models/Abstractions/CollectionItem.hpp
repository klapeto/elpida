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
	public:

		const T& getValue() const
		{
			return _value;
		}

		CollectionItem(const CollectionItem<T>&) = default;
		CollectionItem(CollectionItem<T>&&) noexcept = default;
		CollectionItem<T>& operator=(const CollectionItem<T>&) = delete;
		CollectionItem<T>& operator=(CollectionItem<T>&&) noexcept = default;
	protected:
		const CollectionModel<T>& _owner;
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
