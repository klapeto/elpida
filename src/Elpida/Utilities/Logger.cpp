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
 * Logger.cpp
 *
 *  Created on: 28 Οκτ 2018
 *      Author: klapeto
 */

#include "Elpida/Utilities/Logger.hpp"
#include <iomanip>

namespace Elpida
{

	void Logger::appendTimestamp(std::ostream& out)
	{
		time_t tim;
		time(&tim);
		tm* time = localtime(&tim);
		if (time != nullptr)
		{
			out << '[' << std::setfill('0') << std::setw(2) << time->tm_mday << "/" << std::setfill('0') << std::setw(2)
				<< 1 + time->tm_mon << "/"
				<< 1900 + time->tm_year << " " << std::setfill('0') << std::setw(2) << time->tm_hour << ":"
				<< std::setfill('0')
				<< std::setw(2) << time->tm_min << ":" << std::setfill('0') << std::setw(2) << time->tm_sec << "] ";
		}
	}

	void Logger::appendLogType(LogType type, std::ostream& out)
	{
		out << '[';
		switch (type)
		{
		case LogType::Info:
			out << "Info";
			break;
		case LogType::Warning:
			out << "Warning";
			break;
		case LogType::Error:
			out << "Error";
			break;
		default:
			out << "?????";
			break;
		}
		out << "] -> ";
	}

} /* namespace Elpida */

