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

#include "QtThreadQueue.hpp"
#include <QApplication>
#include <QWindow>
#include <QAbstractEventDispatcher>
#include <QEventLoop>

namespace Elpida::Application
{
	void QtThreadQueue::Enqueue(std::function<void()> func)
	{
		// TODO: Same thread optimization
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