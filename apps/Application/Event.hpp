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

		Event(const Event&) = delete;

		Event(Event&& other) noexcept = default;

		Event& operator=(const Event&) = delete;

		Event& operator=(Event&&) noexcept = default;

		~Event() = default;

	private:
		std::shared_ptr<CallbackCollection> _subscriptions;
	};

} // Application

#endif //ELPIDA_EVENT_HPP_
