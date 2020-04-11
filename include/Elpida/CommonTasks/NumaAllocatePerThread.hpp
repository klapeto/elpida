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
 * NumaAllocatePerThread.hpp
 *
 *  Created on: 9 Ιουν 2019
 *      Author: klapeto
 */

#ifndef ELPIDA_COMMONTASKS_NUMAALLOCATEPERTHREAD_HPP_
#define ELPIDA_COMMONTASKS_NUMAALLOCATEPERTHREAD_HPP_

#include <cstddef>
#include <unordered_map>

#include "Elpida/Task.hpp"
#include "Elpida/TaskRunResult.hpp"

namespace Elpida
{
	class Memory;

	class NumaAllocatePerThread final : public Task
	{
	public:

		const std::unordered_map<int, Memory*>& getAllocatedMemoryRegions() const
		{
			return _allocatedMemoryRegions;
		}

		void prepare() override;
		void run() override;
		void calculateResults(const TaskMetrics& metrics) override;

		explicit NumaAllocatePerThread(std::size_t memorySizePerThread);
		~NumaAllocatePerThread() override;
	private:
		std::unordered_map<int, Memory*> _allocatedMemoryRegions;
		TaskRunResult _result;
		std::size_t _memorySizePerThread;

	};

} /* namespace Elpida */

#endif /* ELPIDA_COMMONTASKS_NUMAALLOCATEPERTHREAD_HPP_ */
