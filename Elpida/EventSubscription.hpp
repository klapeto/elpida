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
 * EventSubscription.hpp
 *
 *  Created on: 13 Οκτ 2018
 *      Author: klapeto
 */

#ifndef ELPIDA_EVENTSUBSCRIPTION_HPP_
#define ELPIDA_EVENTSUBSCRIPTION_HPP_

#include "Elpida/Types/List.hpp"

namespace Elpida
{
	template<typename T>
	class Event;

	template<typename T>
	class EventSubscription
	{
		public:
			typedef void (*EventHandler)(T&);

			void unsubscribe()
			{
				_owner.unsubScribe(*this);
			}

			~EventSubscription()
			{

			}
		private:
			typedef typename List<EventSubscription<T>>::iterator Iterator;

			Iterator _iterator;
			Event<T>& _owner;
			EventHandler _handler;

			EventSubscription(Event<T>& owner, EventHandler hanlder)
					: _owner(owner), _handler(hanlder)
			{

			}

			void setIterator(Iterator iterator)
			{
				_iterator = iterator;
			}

			Iterator getIterator() const
			{
				return _iterator;
			}

			void operator()(T& args) const
			{
				_handler(args);
			}

			friend class Event<T> ;
	};

} /* namespace Elpida */

#endif /* ELPIDA_EVENTSUBSCRIPTION_HPP_ */
