//
// Created by klapeto on 17/4/20.
//

#ifndef APPS_QT_MODELS_ABSTRACTIONS_COLLECTIONMODELOBSERVER_HPP
#define APPS_QT_MODELS_ABSTRACTIONS_COLLECTIONMODELOBSERVER_HPP


#include "CollectionModel.hpp"
#include "EventArgs/CollectionChangedEventArgs.hpp"

namespace Elpida
{

	template<typename T>
	class CollectionModelObserver
	{
	public:

		explicit CollectionModelObserver(const CollectionModel<T>& model)
			: _model(model)
		{
			_subscriptions.push_back(&_model.itemAdded.subscribe([this](const auto& item)
			{
				onItemAdded(item.getItem().getValue());
			}));
			_subscriptions.push_back(&_model.itemRemoved.subscribe([this](const auto& item)
			{
				onItemRemoved(item.getItem().getValue());
			}));
			_subscriptions.push_back(&_model.cleared.subscribe([this]()
			{
				onCollectionCleared();
			}));
		}

		virtual ~CollectionModelObserver()
		{
			for (auto subscription: _subscriptions)
			{
				subscription->unsubscribe();
			}
		}

	private:
		std::vector<EventSubscriptionBase*> _subscriptions;
		const CollectionModel<T>& _model;
	protected:
		virtual void onItemAdded(const T& item)
		{

		}
		virtual void onItemRemoved(const T& item)
		{

		}
		virtual void onCollectionCleared()
		{

		}
	};

}


#endif //APPS_QT_MODELS_ABSTRACTIONS_COLLECTIONMODELOBSERVER_HPP
