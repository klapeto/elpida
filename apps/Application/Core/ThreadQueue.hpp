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

		virtual void Enqueue(std::function<void()> func) = 0;
		virtual int Run() = 0;

		ThreadQueue() = default;
		ThreadQueue(const ThreadQueue&) = delete;
		ThreadQueue& operator=(const ThreadQueue&) = delete;
		ThreadQueue(ThreadQueue&&) noexcept= delete;
		ThreadQueue& operator=(ThreadQueue&&) noexcept= delete;
		virtual ~ThreadQueue() = default;
	};

} // Application

#endif //ELPIDA_THREADQUEUE_HPP
