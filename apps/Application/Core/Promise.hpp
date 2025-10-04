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
// Created by klapeto on 4/5/2023.
//

#ifndef ELPIDA_PROMISE_HPP
#define ELPIDA_PROMISE_HPP

#if __cplusplus > 202002L

#include <future>
#include <functional>
#include <coroutine>
#include <atomic>
#include <thread>
#include "ThreadQueue.hpp"

namespace Elpida::Application
{

	template<typename T = void>
	class Promise
	{
	public:
		struct promise_type
		{
		private:
			std::promise<T> _promise;
			std::coroutine_handle<> _continuation = {};
		public:
			Promise<T> get_return_object()
			{
				return Promise<T>(_promise.get_future(), _continuation);
			}

			constexpr std::suspend_never initial_suspend() const noexcept
			{
				return {};
			}

			constexpr std::suspend_never final_suspend() const noexcept
			{
				return {};
			}

			void return_value(const T& value)
			{
				_promise.set_value(value);
				if (_continuation)
				{
					_continuation();
				}
			}

			void return_value(T&& value)
			{
				_promise.set_value(std::move(value));
				if (_continuation)
				{
					_continuation();
				}
			}

			void unhandled_exception()
			{
				_promise.set_exception(std::current_exception());
				if (_continuation)
				{
					_continuation();
				}
			}
		};

		void await_suspend(std::coroutine_handle<> continuation) noexcept
		{
			_continuation = continuation;
		}

		bool await_ready() const noexcept
		{
			return _future.wait_for(std::chrono::microseconds(0)) != std::future_status::timeout;
		}

		T await_resume()
		{
			return _future.get();
		}

		T Wait()
		{
			return _future.get();
		}

	private:
		explicit Promise(std::future<T> future, std::coroutine_handle<>& continuation)
			: _future(std::move(future)), _continuation(continuation)
		{

		}

		std::future<T> _future;
		std::coroutine_handle<>& _continuation;
	};

	template<>
	class Promise<void>
	{
	public:
		struct promise_type
		{
		private:
			std::promise<void> _promise;
			std::coroutine_handle<> _continuation = {};
		public:
			Promise<void> get_return_object()
			{
				return Promise<void>(_promise.get_future(), _continuation);
			}

			constexpr std::suspend_never initial_suspend() const noexcept
			{
				return {};
			}

			constexpr std::suspend_never final_suspend() const noexcept
			{
				return {};
			}

			void return_void()
			{
				_promise.set_value();
				if (_continuation)
				{
					_continuation();
				}
			}

			void unhandled_exception()
			{
				_promise.set_exception(std::current_exception());
				if (_continuation)
				{
					_continuation();
				}
			}
		};

		void await_suspend(std::coroutine_handle<> continuation) noexcept
		{
			_continuation = continuation;
		}

		bool await_ready() const
		{
			return _future.wait_for(std::chrono::microseconds(0)) != std::future_status::timeout;
		}

		void await_resume()
		{
			_future.get();
		}

		void Wait()
		{
			return _future.get();
		}

	private:
		explicit Promise(std::future<void> future, std::coroutine_handle<>& continuation)
			: _future(std::move(future)), _continuation(continuation)
		{

		}

		std::future<void> _future;
		std::coroutine_handle<>& _continuation;
	};

	template<typename T = void>
	class AsyncPromise
	{
	public:
		bool await_ready()
		{
			return _future.wait_for(std::chrono::microseconds(0)) != std::future_status::timeout;
		}

		void await_suspend(std::coroutine_handle<> continuation)
		{
			_continuationSet = true;
			_continuationPromise.set_value(continuation);
		}

		T await_resume()
		{
			if (!_continuationSet)
			{
				_continuationPromise.set_value({});
			}
			return _future.get();
		}

		template<typename TCallable, typename ... TArgs, typename std::enable_if_t<std::is_invocable_v<TCallable, TArgs...>,bool> = true>
		explicit AsyncPromise(TCallable&& callable, TArgs&& ... args)
		{
			_future = _promise.get_future();
			std::thread(&AsyncPromise<T>::AsyncProcedure<TCallable, TArgs...>, this, ThreadQueue::Current(),
				std::forward<TCallable>(callable), std::forward<TArgs>(args)...).detach();
		}

	private:
		std::promise<std::coroutine_handle<>> _continuationPromise;
		std::promise<T> _promise;
		std::future<T> _future;
		bool _continuationSet = false;

		template<typename TCallable, typename ... TArgs>
		void AsyncProcedure(std::weak_ptr<ThreadQueue> continuationThread, TCallable&& callable, TArgs&& ... args)
		{
			try
			{
				if constexpr (std::is_void_v<T>)
				{
					callable(std::forward<TArgs>(args)...);
					_promise.set_value();
				}
				else
				{
					_promise.set_value(callable(std::forward<TArgs>(args)...));
				}
			}
			catch (...)
			{
				_promise.set_exception(std::current_exception());
			}
			auto continuation = _continuationPromise.get_future().get();
			if (continuation)
			{
				if (auto thread = continuationThread.lock())
				{
					thread->Enqueue([continuation]()
					{
					  continuation();
					});
				}
				else
				{
					continuation();
				}
			}
		}
	};

} // Application

#endif //ELPIDA_PROMISE_HPP

#endif //  __cplusplus > 202002L