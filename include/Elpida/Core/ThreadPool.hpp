//
// Created by klapeto on 31/3/2024.
//

#ifndef ELPIDA_THREADPOOL_HPP
#define ELPIDA_THREADPOOL_HPP

#include "Elpida/Core/Vector.hpp"
#include "Elpida/Core/Ref.hpp"
#include "Elpida/Core/UniquePtr.hpp"
#include "Elpida/Core/BlockingCollection.hpp"
#include "Elpida/Core/Topology/ProcessingUnitNode.hpp"
#include "Elpida/Core/ThreadPoolThread.hpp"

#include <future>
#include <mutex>
#include <functional>
#include <queue>
#include <stack>
#include <ranges>

namespace Elpida
{
	class ThreadPool
	{
		template<typename T, typename TCallable>
		class Function : public ThreadPoolThread::Function
		{
		public:
			void operator()() override
			{
				try
				{
					_promise.set_value(_callable());
				}
				catch (...)
				{
					_promise.set_exception(std::current_exception());
				}
				_threadPool.RequeueThread(std::move(_thread));
			}

			Function(std::promise<T>&& promise, std::unique_ptr<ThreadPoolThread>&& thread, ThreadPool& threadPool, TCallable&& callable)
			: _promise(std::move(promise)), _callable(std::move(callable)), _thread(std::move(thread)), _threadPool(threadPool)
			{

			}

			Function(Function<T, TCallable>&& other) noexcept
			: _promise(std::move(other._promise)), _callable(std::move(other._callable)), _thread(std::move(other._thread)), _threadPool(other._threadPool){

			}

		private:
			std::promise<T> _promise;
			TCallable _callable;
			std::unique_ptr<ThreadPoolThread> _thread;
			ThreadPool& _threadPool;
		};

		template<typename TCallable>
		class FunctionV : public ThreadPoolThread::Function
		{
		public:
			void operator()() override
			{
				try
				{
					_callable();
					_promise.set_value();
				}
				catch (...)
				{
					_promise.set_exception(std::current_exception());
				}
				_threadPool.RequeueThread(std::move(_thread));
			}

			FunctionV(std::promise<void>&& promise, std::unique_ptr<ThreadPoolThread>&& thread, ThreadPool& threadPool, TCallable&& callable)
					: _promise(std::move(promise)), _callable(std::move(callable)), _thread(std::move(thread)), _threadPool(threadPool)
			{

			}

			FunctionV(FunctionV<TCallable>&& other) noexcept
					: _promise(std::move(other._promise)), _callable(std::move(other._callable)), _thread(std::move(other._thread)), _threadPool(other._threadPool){

			}

		private:
			std::promise<void> _promise;
			TCallable _callable;
			std::unique_ptr<ThreadPoolThread> _thread;
			ThreadPool& _threadPool;
		};

	public:
		template<typename T, typename TCallable>
		std::future<T> Queue(TCallable callable)
		{
			std::promise<T> promise;
			auto future = promise.get_future();

			auto th = GetNextThread();
			auto ptr = th.get();

			ptr->Excecute(std::make_unique<Function<T, TCallable>>(std::move(promise), std::move(th), *this, std::move(callable)));

			return std::move(future);
		}

		[[nodiscard]]
		const std::queue<std::unique_ptr<ThreadPoolThread>>& GetQueue() const
		{
			return _threadQueue;
		}

		template<typename TCallable>
		std::future<void> Queue(TCallable callable)
		{
			std::promise<void> promise;
			auto future = promise.get_future();

			auto th = GetNextThread();
			auto ptr = th.get();

			ptr->Excecute(std::make_unique<FunctionV<TCallable>>(std::move(promise), std::move(th), *this, std::move(callable)));

			return std::move(future);
		}

		explicit ThreadPool(const Vector<Ref<const ProcessingUnitNode>>& targetProcessors)
				: _targetProcessors(targetProcessors), _currentThreadId(0)
		{
			for (std::size_t i = 0; i < _targetProcessors.size(); ++i)
			{
				_threadQueue.emplace(CreateNewThread());
			}
		}

		explicit ThreadPool(std::ranges::input_range auto&& targetProcessors)
				: _currentThreadId(0)
		{
			for (auto& processor: targetProcessors)
			{
				_targetProcessors.push_back(processor);
			}
			for (std::size_t i = 0; i < _targetProcessors.size(); ++i)
			{
				_threadQueue.emplace(CreateNewThread());
			}
		}


		~ThreadPool() = default;

	private:
		std::mutex _mutex;
		std::vector<Ref<const ProcessingUnitNode>> _targetProcessors;
		std::queue<std::unique_ptr<ThreadPoolThread>> _threadQueue;
		std::size_t _currentThreadId;

		std::unique_ptr<ThreadPoolThread> GetNextThread()
		{
			std::unique_lock<std::mutex> lock(_mutex);
			if (_threadQueue.empty())
			{
				return CreateNewThread();
			}
			auto th = std::move(_threadQueue.front());
			_threadQueue.pop();
			return std::move(th);
		}

		std::unique_ptr<ThreadPoolThread> CreateNewThread()
		{
			if (_currentThreadId >= _targetProcessors.size())
			{
				_currentThreadId = 0;
			}

			auto targetThreadId=  _currentThreadId++;

			return std::make_unique<ThreadPoolThread>(_targetProcessors[targetThreadId]);
		}

		void RequeueThread(std::unique_ptr<ThreadPoolThread>&& thread)
		{
			std::unique_lock<std::mutex> lock(_mutex);
			if (_threadQueue.size() < _targetProcessors.size())
			{
				_threadQueue.push(std::move(thread));
			}
		}
	};

} // Elpida

#endif //ELPIDA_THREADPOOL_HPP
