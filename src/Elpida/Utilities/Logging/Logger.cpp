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

/*
 * Logger.cpp
 *
 *  Created on: 28 Οκτ 2018
 *      Author: klapeto
 */

#include "Elpida/Utilities/Logging/Logger.hpp"

#include "Elpida/Utilities/Logging/LogAppender.hpp"

namespace Elpida
{

	void Logger::log(LogType type, const std::string& message)
	{
		logImpl(type, std::chrono::system_clock::now(), message, nullptr);
	}

	void Logger::log(LogType type, const std::string& message, const std::exception& exception)
	{
		logImpl(type, std::chrono::system_clock::now(), message, &exception);
	}

	void Logger::logImpl(LogType type,
		TimeStamp timeStamp,
		const std::string& message,
		const std::exception* exception)
	{
		for (auto appender: _appenders)
		{
			appender->append(type, timeStamp, message, exception);
		}
	}

	void Logger::addAppender(LogAppender& appender)
	{
		_appenders.push_back(&appender);
	}

} /* namespace Elpida */

