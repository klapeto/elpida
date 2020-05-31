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

/*
 * Event.hpp
 *
 *  Created on: 13 Οκτ 2018
 *      Author: klapeto
 */

#ifndef ELPIDA_EVENT_HPP_
#define ELPIDA_EVENT_HPP_

#include "Elpida/EventSubscription.hpp"
#include <mutex>
#include <list>

namespace Elpida
{

	template<typename ... T>
	class Event final
	{
	public:
		using EventHandler = typename EventSubscription<T...>::EventHandler;

		void raise(T ... eventArguments)
		{
			std::lock_guard<std::mutex> lock(_mutex);
			for (const auto& subscriber : _subscribers)
			{
				subscriber(eventArguments...);
			}
		}

		template<typename TCallable>
		EventSubscription<T...>& subscribe(TCallable handler) const
		{
			std::lock_guard<std::mutex> lock(_mutex);
			auto itr = _subscribers.insert(_subscribers.end(), EventSubscription<T...>(*this, EventHandler(handler)));
			itr->setIterator(itr);
			return *itr;
		}

		void unsubScribe(EventSubscription<T...>& subscription) const
		{
			std::lock_guard<std::mutex> lock(_mutex);
			_subscribers.erase(subscription.getIterator());
		}


		Event() = default;

		~Event()
		{
			std::lock_guard<std::mutex> lock(_mutex);
			_subscribers.clear();
		}

	private:
		mutable std::mutex _mutex;
		mutable std::list<EventSubscription<T...>> _subscribers;
	};

} /* namespace Elpida */

#endif /* ELPIDA_EVENT_HPP_ */
