//
// Created by klapeto on 2/4/2024.
//

#include "Elpida/Core/ThreadPool/ThreadPoolThread.hpp"

namespace Elpida
{

	void ThreadPoolThread::Excecute(std::unique_ptr<Functor>&& function)
	{
		std::unique_lock<std::mutex> lock(_mutex);
		_function = std::move(function);
		_execute = true;
		_notifier.notify_all();
	}

	void ThreadPoolThread::Terminate()
	{
		std::unique_lock<std::mutex> lock(_mutex);
		_keepGoing = false;
		_function.reset();
		_execute = true;
		_notifier.notify_all();
		lock.unlock();

		_thread.join();
	}

	ThreadPoolThread::ThreadPoolThread()
			: _execute(false), _keepGoing(true)
	{
		_thread = std::thread(&ThreadPoolThread::Procedure, this);
	}

	ThreadPoolThread::~ThreadPoolThread()
	{
		Terminate();
	}

	void ThreadPoolThread::Procedure()
	{
		while (_keepGoing)
		{
			std::unique_lock<std::mutex> lock(_mutex);
			while (!_execute)
			{
				_notifier.wait(lock);
			}
			_execute = false;
			auto function = std::move(_function);
			lock.unlock();
			if (function)
			{
				(*function)();
			}
		}
	}
} // Elpida