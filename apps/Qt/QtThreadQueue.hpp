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

#ifndef ELPIDA_QTTHREADQUEUE_HPP
#define ELPIDA_QTTHREADQUEUE_HPP

#include "Core/ThreadQueue.hpp"

#include <QObject>
#include <deque>
#include <mutex>

namespace Elpida::Application
{

	class QtThreadQueue final : public QObject, public ThreadQueue
	{
	Q_OBJECT
	public:
		void Enqueue(std::function<void()> func) override;

		int Run() override;

		QtThreadQueue();
		~QtThreadQueue() override = default;
	 signals:
		void ProcedureEnqueued();
	private slots:
		void OnProcedureEnqueued();
	private:
		std::mutex mutex;
		std::deque<std::function<void()>> _queue;
	};

} // Application

#endif //ELPIDA_QTTHREADQUEUE_HPP
