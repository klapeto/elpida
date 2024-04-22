//
// Created by klapeto on 31/3/2024.
//

#include "Elpida/Core/ThreadPool/ThreadPool.hpp"

namespace Elpida
{

	ThreadPool::ThreadPool(std::size_t dependentThreadCount, std::size_t independentThreadCount)
			: _maxThreads(dependentThreadCount), _keepGoing(true), _currentIndependentIndex(0)
	{
		//_cleanUpThread = std::thread(&ThreadPool::ThreadCleanupProcedure, this);
		for (std::size_t i = 0; i < dependentThreadCount; ++i)
		{
			_dependedQueue.emplace(CreateNewThread());
		}

		for (std::size_t i = 0; i < independentThreadCount; ++i)
		{
			_independentQueue.push_back(CreateNewThread());
		}
	}

	std::unique_ptr<ThreadPoolThread> ThreadPool::GetNextDependedThread()
	{
		std::unique_lock<std::mutex> lock(_mutex);
		if (_dependedQueue.empty())
		{
			return CreateNewThread();
		}
		auto th = std::move(_dependedQueue.front());
		_dependedQueue.pop();
		return th;
	}

	std::unique_ptr<ThreadPoolThread> ThreadPool::CreateNewThread()
	{
		return std::make_unique<ThreadPoolThread>();
	}

	void ThreadPool::RequeueThread(std::unique_ptr<ThreadPoolThread>&& thread)
	{
		std::unique_lock<std::mutex> lock(_mutex);
//		if (_dependedQueue.size() < _maxThreads)
//		{
			_dependedQueue.push(std::move(thread));
//		}
//		else
//		{
//			_threadsToClean.Add(std::move(thread));
//		}
	}

	ThreadPool::~ThreadPool()
	{
		_keepGoing = false;
		_threadsToClean.Destroy();
		if(_cleanUpThread.joinable())
		{
			_cleanUpThread.join();
		}

	}

	ThreadPool::ThreadPool()
			: ThreadPool(std::thread::hardware_concurrency(),  std::thread::hardware_concurrency())
	{

	}

	void ThreadPool::ThreadCleanupProcedure()
	{
		while (_keepGoing)
		{
			_threadsToClean.Get();
		}
	}

	std::size_t ThreadPool::GetNextIndependentIndex()
	{
		std::unique_lock<std::mutex> lock(_mutex);
		if (++_currentIndependentIndex >= _independentQueue.size())
		{
			_currentIndependentIndex = 0;
		}
		return _currentIndependentIndex;
	}
} // Elpida