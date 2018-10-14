/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2018  Ioannis Panagiotopoulos
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
 * ObjectEvent.hpp
 *
 *  Created on: 13 Οκτ 2018
 *      Author: klapeto
 */

#ifndef ELPIDA_EVENT_HPP_
#define ELPIDA_EVENT_HPP_

#include "Elpida/Types/List.hpp"
#include "Elpida/EventSubscription.hpp"

namespace Elpida
{

	template<typename T>
	class Event
	{
		public:
			typedef typename EventSubscription<T>::EventHandler EventHandler;

			void raise(T& eventArguments)
			{
				for (const auto& subscriber : _subscribers)
				{
					subscriber(eventArguments);
				}
			}

			EventSubscription<T>& subscribe(EventHandler handler)
			{
				auto itr = _subscribers.insert(EventSubscription<T>(*this), handler);
				itr->setIterator(itr);
				return *itr;
			}

			void unsubScribe(EventSubscription<T>& subscription)
			{
				_subscribers.remove(subscription.getIterator());
			}

			Event()
			{

			}
			~Event()
			{

			}

		private:
			List<EventSubscription<T>> _subscribers;

	};

} /* namespace Elpida */

#endif /* ELPIDA_EVENT_HPP_ */
