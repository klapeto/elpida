//
// Created by klapeto on 10/9/2023.
//

#ifndef ELPIDA_SRC_ELPIDA_CORE_POOL_HPP
#define ELPIDA_SRC_ELPIDA_CORE_POOL_HPP

#include <queue>
#include <mutex>
#include <condition_variable>

namespace Elpida
{

	template<typename T>
	class Pool;

	template<typename T>
	class PoolItem final
	{
	public:

		T& Get()
		{
			return _item;
		}

		PoolItem(const PoolItem&) = delete;
		PoolItem(PoolItem&&) noexcept = delete;
		PoolItem& operator=(const PoolItem&) noexcept = delete;
		PoolItem& operator=(PoolItem&&) noexcept = delete;
		~PoolItem()
		{
			_pool.Add(std::move(_item));
		}
	private:

		PoolItem(Pool<T>& pool, T&& item)
			: _item(std::move(item)), _pool(pool)
		{

		}
		T _item;
		Pool<T>& _pool;

		friend class Pool<T>;
	};

	template<typename T>
	class Pool final
	{
	public:

		PoolItem<T> Rent()
		{
			std::unique_lock<std::mutex> lock(_mutex);

			while (_queue.empty())
			{
				_addedConditionVariable.wait(lock);
			}

			auto item = _queue.front();
			_queue.pop();
			return PoolItem<T>(*this, std::move(item));
		}

		void Add(T item)
		{
			std::unique_lock<std::mutex> lock(_mutex);
			_queue.push(item);
			_addedConditionVariable.notify_all();
		}

		template<typename R>
		void Add(const R& collection)
		{
			std::unique_lock<std::mutex> lock(_mutex);
			for (auto& i : collection)
			{
				_queue.push(i);
			}
			_addedConditionVariable.notify_all();
		}

		Pool() = default;
		Pool(const Pool&) = delete;
		Pool(Pool&&) noexcept = delete;
		Pool& operator=(Pool&&) noexcept = delete;
		Pool& operator=(const Pool&) = delete;
		~Pool() = default;
	private:
		std::mutex _mutex;
		std::condition_variable _addedConditionVariable;
		std::queue<T> _queue;
	};

} // Elpida

#endif //ELPIDA_SRC_ELPIDA_CORE_POOL_HPP
