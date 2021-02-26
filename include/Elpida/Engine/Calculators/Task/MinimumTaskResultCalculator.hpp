/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2021  Ioannis Panagiotopoulos
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
// Created by klapeto on 7/2/21.
//

#ifndef SRC_ELPIDA_ENGINE_CALCULATORS_TASK_MINIMUMTASKRESULTCALCULATOR_HPP
#define SRC_ELPIDA_ENGINE_CALCULATORS_TASK_MINIMUMTASKRESULTCALCULATOR_HPP

#include "Elpida/Engine/Calculators/TaskResultCalculator.hpp"

namespace Elpida
{
	class MinimumTaskResultCalculator : public TaskResultCalculator
	{
	public:
		[[nodiscard]]
		ProcessedTaskResult calculateAggregateResult(const std::vector<TaskResult>& taskResults) const override;

		MinimumTaskResultCalculator() = default;
		~MinimumTaskResultCalculator() override = default;
	};
}

#endif //SRC_ELPIDA_ENGINE_CALCULATORS_TASK_MINIMUMTASKRESULTCALCULATOR_HPP
