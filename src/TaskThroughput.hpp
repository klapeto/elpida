/*
 * TaskMeasurement.hpp
 *
 *  Created on: 18 Μαρ 2018
 *      Author: klapeto
 */

#ifndef SRC_TASKTHROUGHPUT_HPP_
#define SRC_TASKTHROUGHPUT_HPP_

#include <string>

namespace Elpida
{

	class TaskThroughput
	{
		public:

			static std::string getValueScale(double value);

			const std::string& getThroughputString() const
			{
				return _throutputString;
			}

			TaskThroughput()
			{

			}

			TaskThroughput(const std::string& measurementString) :
					_throutputString(measurementString)
			{

			}
			virtual ~TaskThroughput()
			{

			}

			TaskThroughput(TaskThroughput&&) = default;
			TaskThroughput(const TaskThroughput&) = default;
			TaskThroughput& operator=(TaskThroughput&&) = default;
			TaskThroughput& operator=(const TaskThroughput&) = default;
		private:
			std::string _throutputString;
	};

} /* namespace Elpida */

#endif /* SRC_TASKTHROUGHPUT_HPP_ */
