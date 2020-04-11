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
 * MemoryTasksProperties.hpp
 *
 *  Created on: 16 Μαΐ 2019
 *      Author: klapeto
 */

#ifndef TASKBATCHES_MEMORY_CHARTS_MEMORYTASKSPROPERTIES_HPP_
#define TASKBATCHES_MEMORY_CHARTS_MEMORYTASKSPROPERTIES_HPP_

#include "TaskBatches/QtTaskBatchWrapper.hpp"

namespace Elpida
{

	template<typename T>
	class MemoryTasksProperties final : public QtTaskBatchWrapper
	{
	public:

		[[nodiscard]] const TaskBatch& getTaskBatch() const override
		{
			return _taskBatch;
		}

		void reconfigureTaskBatch() override
		{

		}
		void validateConfiguration() override
		{

		}

		explicit MemoryTasksProperties(T&& batch)
			: QtTaskBatchWrapper(false, false), _taskBatch(std::move(batch))
		{

		}

		~MemoryTasksProperties() override = default;
	private:
		T _taskBatch;
	};

} /* namespace Elpida */

#endif /* TASKBATCHES_MEMORY_CHARTS_MEMORYTASKSPROPERTIES_HPP_ */
