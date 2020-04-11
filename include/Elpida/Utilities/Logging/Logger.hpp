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
 * Logger.hpp
 *
 *  Created on: 28 Οκτ 2018
 *      Author: klapeto
 */

#ifndef ELPIDA_UTILITIES_LOGGING_LOGGER_HPP_
#define ELPIDA_UTILITIES_LOGGING_LOGGER_HPP_

#include <vector>
#include <chrono>
#include <string>

namespace Elpida
{
	enum class LogType
	{
		Info, Warning, Error
	};

	class LogAppender;

	class Logger final
	{
	public:


		using TimeStamp = std::chrono::time_point<std::chrono::system_clock>;

		void addAppender(LogAppender& appender);

		void log(LogType type, const std::string& message);
		void log(LogType type, const std::string& message, const std::exception& exception);

		Logger() = default;
		~Logger() = default;
	private:
		std::vector<LogAppender*> _appenders;

		void logImpl(LogType type, TimeStamp timeStamp, const std::string& message, const std::exception* exception);
	};

} /* namespace Elpida */

#endif /* ELPIDA_UTILITIES_LOGGING_LOGGER_HPP_ */
