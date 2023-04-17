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
