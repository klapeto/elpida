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

#ifndef ELPIDA_EVENTSUBSCRIPTION_HPP_
#define ELPIDA_EVENTSUBSCRIPTION_HPP_

#include <list>
#include <functional>
#include <memory>

namespace Elpida::Application
{
	template<typename ...T>
	class EventSubscription final
	{
	 public:
		using SubscriptionList = std::list<std::function<void(T...)>>;
		using SubscriptionIterator = typename SubscriptionList::iterator;

		void UnSubscribe()
		{
			if (auto ptr = _list.lock())
			{
				ptr->erase(_index);
				_list.reset();
			}
		}

		EventSubscription() = default;
		EventSubscription(std::weak_ptr<SubscriptionList> list, SubscriptionIterator index)
			: _list(std::move(list)), _index(index)
		{

		}
		EventSubscription(const EventSubscription&) = delete;
		EventSubscription& operator=(const EventSubscription&) = delete;
		EventSubscription(EventSubscription&&) noexcept = default;
		EventSubscription& operator=(EventSubscription&&) noexcept = default;
		~EventSubscription()
		{
			UnSubscribe();
		}
	 private:
		std::weak_ptr<SubscriptionList> _list;
		SubscriptionIterator _index;
	};

} // Application

#endif //ELPIDA_EVENTSUBSCRIPTION_HPP_
