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
 * MultiThreadMemoryRead.hpp
 *
 *  Created on: 20 Οκτ 2018
 *      Author: klapeto
 */

#ifndef TASKBATCHES_MEMORY_MULTITHREADMEMORYREAD_HPP_
#define TASKBATCHES_MEMORY_MULTITHREADMEMORYREAD_HPP_

#include <Elpida/MultiThreadTask.hpp>
#include <Elpida/Types/Array.hpp>
#include <Elpida/Types/Primitives.hpp>

namespace Elpida
{
	class Memory;

	class MultiThreadMemoryRead: public MultiThreadTask
	{
		public:

			void calculateResults();

			MultiThreadMemoryRead(const Memory& memory);
			virtual ~MultiThreadMemoryRead();

		protected:
			void createTasks();
		private:
			TaskRunResult _result;
			const Memory& _memory;
	};

} /* namespace Elpida */

#endif /* TASKBATCHES_MEMORY_MULTITHREADMEMORYREAD_HPP_ */
