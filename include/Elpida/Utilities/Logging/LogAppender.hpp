//
// Created by klapeto on 9/4/20.
//

#ifndef ELPIDA_UTILITIES_LOGGING_LOGAPPENDER_HPP
#define ELPIDA_UTILITIES_LOGGING_LOGAPPENDER_HPP

#include <string>
#include "Logger.hpp"
#include <chrono>

namespace Elpida
{
	class LogAppender
	{
	public:

		virtual void append(Logger::LogType logType, const Logger::TimeStamp& timeStamp, const std::string& message, const std::exception* exception) = 0;

		virtual ~LogAppender()
		{

		}
	};
}

#endif //ELPIDA_UTILITIES_LOGGING_LOGAPPENDER_HPP
