//
// Created by klapeto on 4/5/2023.
//

#ifndef ELPIDA_THREADQUEUE_HPP
#define ELPIDA_THREADQUEUE_HPP

#include <memory>
#include <utility>
#include <deque>
#include <functional>
#include <mutex>

namespace Elpida::Application
{

	class ThreadQueue
	{
	public:
		static std::weak_ptr<ThreadQueue> Current();
		static void SetCurrent(std::shared_ptr<ThreadQueue> current);

		void Enqueue(std::function<void()> func);
		void Run();
		void Stop();

		ThreadQueue();
		ThreadQueue(const ThreadQueue&) = delete;
		ThreadQueue& operator=(const ThreadQueue&) = delete;
		ThreadQueue(ThreadQueue&&) noexcept= delete;
		ThreadQueue& operator=(ThreadQueue&&) noexcept= delete;
		virtual ~ThreadQueue() = default;
	protected:
		virtual bool DefaultProcedure() = 0;
	private:
		static inline thread_local std::shared_ptr<ThreadQueue> _current;

		std::mutex _mutex;
		std::deque<std::function<void()>> _queue;
		bool _keepRunning;
	};

} // Application

#endif //ELPIDA_THREADQUEUE_HPP
