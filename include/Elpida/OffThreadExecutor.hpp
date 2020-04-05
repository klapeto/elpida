/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2018  Ioannis Panagiotopoulos
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

/*
 * OffThreadExecutor.hpp
 *
 *  Created on: 13 Οκτ 2018
 *      Author: klapeto
 */

#ifndef ELPIDA_OFFTHREADEXECUTOR_HPP_
#define ELPIDA_OFFTHREADEXECUTOR_HPP_

#include <thread>

#include "Elpida/Exceptions/ElpidaException.hpp"

namespace Elpida
{

	class OffThreadExecutor final
	{
		public:

			template<typename TCallable>
			void run(TCallable&& callable)
			{
				if (_runnerThread.joinable()) throw ElpidaException("OffThreadExecutor",
				                                                    "Attempted to run while thread is already executing");
				_runnerThread = std::thread(callable);
			}

			bool isRunning() const
			{
				return _runnerThread.joinable();
			}

			void waitThreadToExit()
			{
				if (_runnerThread.joinable())
				{
					_runnerThread.join();
				}
			}

			void detach()
			{
				if (_runnerThread.joinable())
				{
					_runnerThread.detach();
				}
			}

			OffThreadExecutor()
			{
			}

			~OffThreadExecutor()
			{
				waitThreadToExit();
			}
		private:
			std::thread _runnerThread;
	};
}
/* namespace Elpida */

#endif /* ELPIDA_OFFTHREADEXECUTOR_HPP_ */
