/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2020  Ioannis Panagiotopoulos
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>
 *************************************************************************/

//
// Created by klapeto on 8/4/20.
//

#ifndef APPS_QT_CORE_ELPIDAMEDIATOR_HPP
#define APPS_QT_CORE_ELPIDAMEDIATOR_HPP

#include "Core/Abstractions/Mediator.hpp"
#include "Core/Abstractions/CommandHandler.hpp"
#include <vector>
#include <functional>
#include <condition_variable>
#include <mutex>
#include <thread>

namespace Elpida
{

	class ElpidaMediator final : public Mediator
	{
	public:
		void execute(Command& command) override;

		void execute(Command&& command) override;

		void registerCommandHandler(CommandHandler& handler);

		void setToWait() override
		{
			_shouldWait = true;
		}

		void waitToContinue() override
		{
			if (!_shouldWait || std::this_thread::get_id() != _mainThreadId) return;
			std::unique_lock<std::mutex> lock(_mutex);
			_shouldWait = false;
			_doWake = false;
			while (!_doWake)
			{
				_conditionVariable.wait(lock);
			}
		}

		void signalToContinue() override
		{
			std::unique_lock<std::mutex> lock(_mutex);
			_doWake = true;
			_conditionVariable.notify_all();
		}

		ElpidaMediator() = default;

		~ElpidaMediator() override = default;

	private:
		std::vector<std::reference_wrapper<CommandHandler>> _handlers;
		std::condition_variable _conditionVariable;
		std::mutex _mutex;
		std::thread::id _mainThreadId = std::this_thread::get_id();
		bool _doWake = false;
		bool _shouldWait = false;
	};
}

#endif //APPS_QT_CORE_ELPIDAMEDIATOR_HPP
