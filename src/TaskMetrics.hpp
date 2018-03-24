/*
 * TaskResult.hpp
 *
 *  Created on: 18 Μαρ 2018
 *      Author: klapeto
 */

#ifndef SRC_TASKMETRICS_HPP_
#define SRC_TASKMETRICS_HPP_

#include <chrono>
#include <cstddef>

namespace Elpida
{

	class TaskMetrics
	{
		public:

			typedef std::chrono::duration<double, std::nano> Duration;

			size_t getClockCycles() const
			{
				return _clockCycles;
			}

			const Duration& getDuration() const
			{
				return _duration;
			}

			double getSeconds() const
			{
				return ((double) _duration.count() / (double) std::nano::den);
			}

			TaskMetrics() :
					_clockCycles(0)
			{

			}

			TaskMetrics(const Duration& duration, size_t clockCycles) :
					_duration(duration),
					_clockCycles(clockCycles)
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
			size_t _clockCycles;
	};

} /* namespace Elpida */

#endif /* SRC_TASKMETRICS_HPP_ */
