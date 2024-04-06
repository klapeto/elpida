//
// Created by klapeto on 31/3/2024.
//

#include "Elpida/Core/ThreadPool/ThreadPool.hpp"

namespace Elpida
{

	ThreadPool::ThreadPool(std::size_t threadCount)
			: _maxThreads(threadCount), _keepGoing(true)
	{
		_cleanUpThread = std::thread(&ThreadPool::ThreadCleanupProcedure, this);
		for (std::size_t i = 0; i <_maxThreads; ++i)
		{
			_threadQueue.emplace(CreateNewThread());
		}
	}

	std::unique_ptr<ThreadPoolThread> ThreadPool::GetNextThread()
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

	std::unique_ptr<ThreadPoolThread> ThreadPool::CreateNewThread()
	{
		return std::make_unique<ThreadPoolThread>();
	}

	void ThreadPool::RequeueThread(std::unique_ptr<ThreadPoolThread>&& thread)
	{
		std::unique_lock<std::mutex> lock(_mutex);
		if (_threadQueue.size() < _maxThreads)
		{
			_threadQueue.push(std::move(thread));
		}
		else
		{
			_threadsToClean.Add(std::move(thread));
		}
	}

	ThreadPool::~ThreadPool()
	{
		_keepGoing = false;
		_threadsToClean.Destroy();
		_cleanUpThread.join();
	}

	ThreadPool::ThreadPool()
			: ThreadPool(std::thread::hardware_concurrency())
	{

	}

	void ThreadPool::ThreadCleanupProcedure()
	{
		while (_keepGoing)
		{
			_threadsToClean.Get();
		}
	}
} // Elpida