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
// Created by klapeto on 17/4/20.
//

#ifndef APPS_QT_MODELS_ABSTRACTIONS_COLLECTIONMODELOBSERVER_HPP
#define APPS_QT_MODELS_ABSTRACTIONS_COLLECTIONMODELOBSERVER_HPP

#include "Models/Abstractions/CollectionModel.hpp"
#include "Models/Abstractions/EventArgs/CollectionChangedEventArgs.hpp"

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
