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

namespace Elpida
{
	class ThreadPool final
	{
		template<typename T, typename TCallable>
		class DependentThreadPoolFunctor : public Functor
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

			DependentThreadPoolFunctor(std::promise<T>&& promise, std::unique_ptr<ThreadPoolThread>&& thread, ThreadPool& threadPool,
					TCallable&& callable)
					: _promise(std::move(promise)), _callable(std::move(callable)), _thread(std::move(thread)),
					  _threadPool(threadPool)
			{

			}

			DependentThreadPoolFunctor(DependentThreadPoolFunctor<T, TCallable>&& other) noexcept
					: _promise(std::move(other._promise)), _callable(std::move(other._callable)),
					  _thread(std::move(other._thread)), _threadPool(other._threadPool)
			{

			}

			~DependentThreadPoolFunctor() override = default;

		private:
			std::promise<T> _promise;
			TCallable _callable;
			std::unique_ptr<ThreadPoolThread> _thread;
			ThreadPool& _threadPool;
		};

		template<typename T, typename TCallable>
		class IndependentThreadPoolFunctor : public Functor
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
			}

			IndependentThreadPoolFunctor(std::promise<T>&& promise,TCallable&& callable)
					: _promise(std::move(promise)), _callable(std::move(callable))
			{

			}

			IndependentThreadPoolFunctor(IndependentThreadPoolFunctor<T, TCallable>&& other) noexcept
					: _promise(std::move(other._promise)), _callable(std::move(other._callable))
			{

			}

			~IndependentThreadPoolFunctor() override = default;

		private:
			std::promise<T> _promise;
			TCallable _callable;
		};

	public:

		template<typename T, typename TCallable>
		std::future<T> Queue(TCallable callable)
		{
			std::promise<T> promise;
			auto future = promise.get_future();

			auto th = GetNextDependedThread();
			auto ptr = th.get();

			ptr->Excecute(std::make_unique<DependentThreadPoolFunctor<T, TCallable>>(std::move(promise),
					std::move(th),
					*this,
					std::move(callable)));

			return std::move(future);
		}

		template<typename T, typename TCallable>
		std::future<T> QueueIndependent(TCallable callable)
		{
			std::promise<T> promise;
			auto future = promise.get_future();

			auto& th = _independentQueue[GetNextIndependentIndex()];
			th->Excecute(std::make_unique<IndependentThreadPoolFunctor<T, TCallable>>(std::move(promise), std::move(callable)));


			return std::move(future);
		}

		[[nodiscard]]
		std::size_t GetAvailableThreads() const
		{
			return _maxThreads;
		}

		ThreadPool();
		explicit ThreadPool(std::size_t dependentThreadCount, std::size_t independentThreadCount);

		~ThreadPool();
	private:
		std::mutex _mutex;
		std::queue<std::unique_ptr<ThreadPoolThread>> _dependedQueue;
		std::vector<std::unique_ptr<ThreadPoolThread>> _independentQueue;
		std::thread _cleanUpThread;
		BlockingCollection<std::unique_ptr<ThreadPoolThread>> _threadsToClean;
		std::size_t _maxThreads;
		std::size_t _currentIndependentIndex;
		bool _keepGoing;

		std::size_t GetNextIndependentIndex();

		std::unique_ptr<ThreadPoolThread> GetNextDependedThread();
		static std::unique_ptr<ThreadPoolThread> CreateNewThread();
		void RequeueThread(std::unique_ptr<ThreadPoolThread>&& thread);
		void ThreadCleanupProcedure();
	};

} // Elpida

#endif //ELPIDA_THREADPOOL_HPP
