/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

//
// Created by klapeto on 15/3/2023.
//

#ifndef ELPIDA_EVENT_HPP_
#define ELPIDA_EVENT_HPP_

#include "EventSubscription.hpp"
#include <functional>
#include <list>
#include <memory>

namespace Elpida::Application
{
	template<typename ... T>
	class Event final
	{
	private:
		using Callback = std::function<void(T...)>;
		using CallbackCollection = std::list<Callback>;

	public:
		void Raise(T...  arguments)
		{
			for (auto& callback: *_subscriptions)
			{
				callback(std::forward<T>(arguments)...);
			}
		}

		template<typename TCallable>
		EventSubscription<T...> Subscribe(TCallable callable)
		{
			return EventSubscription<T...>(_subscriptions, _subscriptions->insert(_subscriptions->end(), callable));
		}

		Event()
		{
			_subscriptions = std::make_shared<CallbackCollection>();
		}

		Event(const Event&){

		}
		Event(Event&& other) noexcept = default;
		Event& operator=(const Event&){
			return *this;
		}
		Event& operator=(Event&&) noexcept = default;
		~Event() = default;
	private:
		std::shared_ptr<CallbackCollection> _subscriptions;
	};

} // Application

#endif //ELPIDA_EVENT_HPP_
