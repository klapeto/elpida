//
// Created by klapeto on 31/3/2024.
//

#include "Elpida/Core/ThreadPool/ThreadPool.hpp"

namespace Elpida
{

	ThreadPool::ThreadPool(const Vector<Ref<const ProcessingUnitNode>>& targetProcessors, bool pin)
			: _targetProcessors(targetProcessors), _currentThreadId(0), _pin(pin), _keepGoing(true)
	{
		_cleanUpThread = std::thread(&ThreadPool::ThreadCleanupProcedure, this);
		for (std::size_t i = 0; i < _targetProcessors.size(); ++i)
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
		if (_currentThreadId >= _targetProcessors.size())
		{
			_currentThreadId = 0;
		}

		auto targetThreadId = _currentThreadId++;

		return std::make_unique<ThreadPoolThread>(_targetProcessors[targetThreadId], _pin);
	}

	void ThreadPool::RequeueThread(std::unique_ptr<ThreadPoolThread>&& thread)
	{
		std::unique_lock<std::mutex> lock(_mutex);
		if (_threadQueue.size() < _targetProcessors.size())
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
			: _currentThreadId(0), _pin(false), _keepGoing(false)
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