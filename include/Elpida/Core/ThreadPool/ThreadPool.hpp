//
// Created by klapeto on 31/3/2024.
//

#ifndef ELPIDA_THREADPOOL_HPP
#define ELPIDA_THREADPOOL_HPP

#include "Elpida/Core/Vector.hpp"
#include "Elpida/Core/Ref.hpp"
#include "Elpida/Core/UniquePtr.hpp"
#include "Elpida/Core/Topology/ProcessingUnitNode.hpp"
#include "Elpida/Core/ThreadPool/Functor.hpp"
#include "Elpida/Core/ThreadPool/ThreadPoolThread.hpp"
#include "Elpida/Core/BlockingCollection.hpp"

#include <future>
#include <mutex>
#include <functional>
#include <queue>
#include <stack>
#include <ranges>

namespace Elpida
{
	class ThreadPool final
	{
		template<typename T, typename TCallable>
		class ThreadPoolFunctor : public Functor
		{
		public:
			void operator()() override
			{
				try
				{
					if constexpr (std::is_void_v<T>)
					{
						_callable();
						_promise.set_value();
					}
					else
					{
						_promise.set_value(_callable());
					}
				}
				catch (...)
				{
					_promise.set_exception(std::current_exception());
				}
				_threadPool.RequeueThread(std::move(_thread));
			}

			ThreadPoolFunctor(std::promise<T>&& promise, std::unique_ptr<ThreadPoolThread>&& thread, ThreadPool& threadPool,
					TCallable&& callable)
					: _promise(std::move(promise)), _callable(std::move(callable)), _thread(std::move(thread)),
					  _threadPool(threadPool)
			{

			}

			ThreadPoolFunctor(ThreadPoolFunctor<T, TCallable>&& other) noexcept
					: _promise(std::move(other._promise)), _callable(std::move(other._callable)),
					  _thread(std::move(other._thread)), _threadPool(other._threadPool)
			{

			}

			~ThreadPoolFunctor() override = default;

		private:
			std::promise<T> _promise;
			TCallable _callable;
			std::unique_ptr<ThreadPoolThread> _thread;
			ThreadPool& _threadPool;
		};

	public:

		template<typename T, typename TCallable>
		std::future<T> Queue(TCallable callable)
		{
			return std::async(std::launch::async, callable);
//			std::promise<T> promise;
//			auto future = promise.get_future();
//
//			auto th = GetNextThread();
//			auto ptr = th.get();
//
//			ptr->Excecute(std::make_unique<ThreadPoolFunctor<T, TCallable>>(std::move(promise),
//					std::move(th),
//					*this,
//					std::move(callable)));
//
//			return std::move(future);
		}

		[[nodiscard]]
		const std::queue<std::unique_ptr<ThreadPoolThread>>& GetQueue() const
		{
			return _threadQueue;
		}

		[[nodiscard]]
		std::size_t GetAvailableThreads() const
		{
			return _maxThreads;
		}

		ThreadPool();
		explicit ThreadPool(std::size_t threadCount);

		~ThreadPool();
	private:
		std::mutex _mutex;
		std::queue<std::unique_ptr<ThreadPoolThread>> _threadQueue;
		std::thread _cleanUpThread;
		BlockingCollection<std::unique_ptr<ThreadPoolThread>> _threadsToClean;
		std::size_t _maxThreads;
		bool _keepGoing;

		std::unique_ptr<ThreadPoolThread> GetNextThread();
		static std::unique_ptr<ThreadPoolThread> CreateNewThread();
		void RequeueThread(std::unique_ptr<ThreadPoolThread>&& thread);
		void ThreadCleanupProcedure();
	};

} // Elpida

#endif //ELPIDA_THREADPOOL_HPP
