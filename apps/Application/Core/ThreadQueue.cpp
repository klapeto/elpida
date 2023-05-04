//
// Created by klapeto on 4/5/2023.
//

#include "ThreadQueue.hpp"

namespace Elpida::Application
{
	void ThreadQueue::SetCurrent(std::shared_ptr<ThreadQueue> current)
	{
		_current = std::move(current);
	}

	std::weak_ptr<ThreadQueue> ThreadQueue::Current()
	{
		return _current;
	}

	void ThreadQueue::Run()
	{
		_keepRunning = true;
		while (_keepRunning)
		{
			std::unique_lock<std::mutex> lock(_mutex);

			while (!_queue.empty())
			{
				auto func = _queue.front();
				_queue.pop_front();
				lock.unlock();
				try
				{
					func();
				}
				catch (...)
				{
					// What to do here?
				}
				lock.lock();
			}
			lock.unlock();
			_keepRunning = DefaultProcedure();
		}
	}

	void ThreadQueue::Enqueue(std::function<void()> func)
	{
		_queue.push_back(std::move(func));
	}

	ThreadQueue::ThreadQueue()
			: _keepRunning(false)
	{

	}

	void ThreadQueue::Stop()
	{
		_keepRunning = false;
	}
} // Application