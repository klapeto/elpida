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
// Created by klapeto on 19/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_RESULT_TASKMETRICS_HPP
#define INCLUDE_ELPIDA_ENGINE_RESULT_TASKMETRICS_HPP

#include <chrono>
#include "Elpida/Utilities/Duration.hpp"

namespace Elpida
{
	class TaskMetrics final
	{
	public:

		[[nodiscard]]
		const Duration& getDuration() const
		{
			return _duration;
		}

		[[nodiscard]]
		double getResultValue() const
		{
			return _resultValue;
		}

		[[nodiscard]]
		size_t getInputDataSize() const
		{
			return _inputDataSize;
		}

		explicit TaskMetrics(const Elpida::Duration& duration, double resultValue, size_t inputDataSize);
		TaskMetrics(TaskMetrics&&) = default;
		TaskMetrics(const TaskMetrics&) = default;
		TaskMetrics& operator=(TaskMetrics&&) = default;
		TaskMetrics& operator=(const TaskMetrics&) = default;
		~TaskMetrics() = default;
	private:
		Duration _duration;
		double _resultValue;
		size_t _inputDataSize;
	};
}

#endif //INCLUDE_ELPIDA_ENGINE_RESULT_TASKMETRICS_HPP
