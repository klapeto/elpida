/*
 * TaskResult.hpp
 *
 *  Created on: 18 Μαρ 2018
 *      Author: klapeto
 */

#ifndef SRC_TASKMETRICS_HPP_
#define SRC_TASKMETRICS_HPP_

#include <chrono>

namespace Elpida
{

	class TaskMetrics
	{
		public:

			typedef std::chrono::duration<double, std::nano> Duration;

			const Duration& getDuration() const
			{
				return _duration;
			}

			double getSeconds() const
			{
				return ((double) _duration.count() / (double) std::nano::den);
			}

			TaskMetrics()
			{

			}

			TaskMetrics(const Duration& duration) :
					_duration(duration)
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

#endif /* SRC_TASKMETRICS_HPP_ */
