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
// Created by klapeto on 14/11/20.
//

#ifndef TASKRESULTCALCULATOR_HPP
#define TASKRESULTCALCULATOR_HPP

#include "Elpida/Engine/Result/TaskResult.hpp"
#include "Elpida/Engine/Result/ProcessedTaskResult.hpp"
#include <vector>

namespace Elpida
{
	class TaskResultCalculator
	{
	public:
		[[nodiscard]]
		virtual ProcessedTaskResult calculateAggregateResult(const std::vector<TaskResult>& taskResults) const = 0;

		TaskResultCalculator() = default;
		virtual ~TaskResultCalculator() = default;
	};
}

#endif //TASKRESULTCALCULATOR_HPP
