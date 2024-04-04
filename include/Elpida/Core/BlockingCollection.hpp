//
// Created by klapeto on 31/3/2024.
//

#ifndef ELPIDA_BLOCKINGCOLLECTION_HPP
#define ELPIDA_BLOCKINGCOLLECTION_HPP

#include <queue>
#include <mutex>
#include <condition_variable>

namespace Elpida
{

	template<typename T>
	class BlockingCollection final
	{
	public:
		T Get()
		{
			std::unique_lock<std::mutex> lock(_mutex);

			while (_queue.empty() && !_stop)
			{
				_notifier.wait(lock);
			}
			if (_stop) return {};
			T value = std::move(_queue.front());
			_queue.pop();
			return std::move(value);
		}

		void Add(T&& value)
		{
			std::unique_lock<std::mutex> lock(_mutex);
			if (_stop) return;
			_queue.push(std::move(value));
			_notifier.notify_all();
		}

		void Destroy()
		{
			std::unique_lock<std::mutex> lock(_mutex);
			_stop = true;
			while (!_queue.empty())
			{
				_queue.pop();
			}
			_notifier.notify_all();
		}

		BlockingCollection() = default;

		~BlockingCollection()
		{
			Destroy();
		}

	private:
		std::queue<T> _queue;
		std::mutex _mutex;
		std::condition_variable _notifier;
		bool _stop = false;
	};

} // Elpida

#endif //ELPIDA_BLOCKINGCOLLECTION_HPP
