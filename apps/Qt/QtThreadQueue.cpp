//
// Created by klapeto on 4/5/2023.
//

#include "QtThreadQueue.hpp"
#include <QApplication>
#include <QWindow>
#include <QAbstractEventDispatcher>
#include <QEventLoop>

namespace Elpida::Application
{
	void QtThreadQueue::Enqueue(std::function<void()> func)
	{
		std::unique_lock<std::mutex> lock(mutex);
		_queue.push_back(std::move(func));
		lock.unlock();
		emit ProcedureEnqueued();
	}

	int QtThreadQueue::Run()
	{
		return QApplication::exec();
	}

	QtThreadQueue::QtThreadQueue()
	{
		QApplication::connect(this, &QtThreadQueue::ProcedureEnqueued, this, &QtThreadQueue::OnProcedureEnqueued);
	}

	void QtThreadQueue::OnProcedureEnqueued()
	{
		std::unique_lock<std::mutex> lock(mutex);
		while (!_queue.empty())
		{
			auto func = _queue.front();
			_queue.pop_front();
			lock.unlock();
			try
			{
				func();
			} catch (...) {

			}
			lock.lock();
		}
	}
} // Application