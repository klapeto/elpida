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

		virtual void append(LogType logType,
			const Logger::TimeStamp& timeStamp,
			const std::string& message,
			const std::exception* exception) = 0;

		virtual ~LogAppender() = default;
	};
}

#endif //ELPIDA_UTILITIES_LOGGING_LOGAPPENDER_HPP
