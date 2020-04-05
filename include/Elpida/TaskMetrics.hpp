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
 * TaskResult.hpp
 *
 *  Created on: 18 Μαρ 2018
 *      Author: klapeto
 */

#ifndef ELPIDA_TASKMETRICS_HPP_
#define ELPIDA_TASKMETRICS_HPP_

#include <chrono>
#include <ratio>

namespace Elpida
{

	class TaskMetrics
	{
	public:

		typedef std::nano NanoSecond;
		typedef std::micro MicroSecond;
		typedef std::milli MilliSecond;
		typedef std::ratio<1, 1> Second;
		typedef std::chrono::duration<double> Duration;

		const Duration& getDuration() const
		{
			return _duration;
		}

		template<typename Division>
		double getSubdivision() const
		{
			return ((double)Division::den * _duration.count()) / (double)Division::num;
		}

		TaskMetrics()
		{

		}

		TaskMetrics(const Duration& duration)
			: _duration(duration)
		{

		}

		virtual ~TaskMetrics()
		{

		}

		TaskMetrics(TaskMetrics&&) = default;
		TaskMetrics(const TaskMetrics&) = default;
		TaskMetrics& operator=(TaskMetrics&&) = default;
		TaskMetrics& operator=(const TaskMetrics&) = default;

	private:
		Duration _duration;
	};

} /* namespace Elpida */

#endif /* ELPIDA_TASKMETRICS_HPP_ */
