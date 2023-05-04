//
// Created by klapeto on 4/5/2023.
//

#ifndef ELPIDA_PROMISE_HPP
#define ELPIDA_PROMISE_HPP

#include <future>
#include <functional>
#include <thread>
#include "ThreadQueue.hpp"

namespace Elpida::Application
{

	template<typename TFunc, typename TThen>
	void Promise(TFunc func, TThen then)
	{
		auto currentThread = ThreadQueue::Current();
		std::thread([func, then, currentThread]()
		{
			try
			{
				auto value = func();
				if (auto queue = currentThread.lock())
				{
					queue->Enqueue([value, then](){then(value);});
				}
				else
				{
					then(value);
				}
			}
			catch (...)
			{
			}
		}).detach();
	}
} // Application

#endif //ELPIDA_PROMISE_HPP
