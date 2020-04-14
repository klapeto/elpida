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

#include <list>
#include "Elpida/Utilities/NonCopyable.hpp"
#include "Elpida/EventSubscriptionBase.hpp"
#include <functional>

namespace Elpida
{
	template<typename ... T>
	class Event;

	template<typename ... T>
	class EventSubscription : public NonCopyable, public EventSubscriptionBase
	{
	public:
		typedef std::function<void(T...)> EventHandler;

		void unsubscribe() override
		{
			_owner.unsubScribe(*this);
		}

		EventSubscription(EventSubscription<T...>&& other) noexcept
			: _iterator(std::move(other._iterator)), _owner(other._owner), _handler(std::move(other._handler))
		{

		}

		~EventSubscription() override = default;
	private:
		using Iterator = typename std::list<EventSubscription<T...>>::iterator;

		Iterator _iterator;
		const Event<T...>& _owner;
		EventHandler _handler;

		EventSubscription(const Event<T...>& owner, EventHandler&& handler)
			: _owner(owner), _handler(std::move(handler))
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

		void operator()(T ... args) const
		{
			_handler(args...);
		}

		friend class Event<T...>;
	};

} /* namespace Elpida */

#endif /* ELPIDA_EVENTSUBSCRIPTION_HPP_ */
