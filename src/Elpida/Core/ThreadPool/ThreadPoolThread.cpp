//
// Created by klapeto on 2/4/2024.
//

#include "Elpida/Core/ThreadPool/ThreadPoolThread.hpp"

namespace Elpida
{
	void ThreadPoolThread::Excecute(std::unique_ptr<Functor>&& function)
	{
		_queue.Add(std::move(function));
	}

	void ThreadPoolThread::Terminate()
	{
		_keepGoing.store(false, std::memory_order::release);
		_queue.Destroy();

		_thread.join();
	}

	ThreadPoolThread::ThreadPoolThread()
			: _keepGoing(true)
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
			auto function = _queue.Get();
			if (function)
			{
				(*function)();
			}
		}
	}
} // Elpida