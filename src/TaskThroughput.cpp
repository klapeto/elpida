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
 * TaskMeasurement.cpp
 *
 *  Created on: 18 Μαρ 2018
 *      Author: klapeto
 */

#include "TaskThroughput.hpp"

#include <iomanip>
#include <sstream>

namespace Elpida
{

	std::string TaskThroughput::getValueScale(double value)
	{
		std::stringstream returnString;
		if (value < 1000.0)
		{
			returnString << std::fixed << std::setprecision(2) << value;
			return returnString.str();
		}

		if (value < 1000000.0)
		{
			returnString << std::fixed << std::setprecision(2) << value / 1000.0 << " K";
			return returnString.str();
		}

		if (value < 1000000000.0)
		{
			returnString << std::fixed << std::setprecision(2) << value / 1000000.0 << " M";
			return returnString.str();
		}

		if (value < 1000000000000.0)
		{
			returnString << std::fixed << std::setprecision(2) << value / 1000000000.0 << " G";
			return returnString.str();
		}

		if (value < 1000000000000000.0)
		{
			returnString << std::fixed << std::setprecision(2) << value / 1000000000000.0 << " T";
			return returnString.str();
		}

		if (value < 1000000000000000000.0)
		{
			returnString << std::fixed << std::setprecision(2) << value / 1000000000000000.0 << " P";
			return returnString.str();
		}
		if (value < 1000000000000000000000.0)
		{
			returnString << std::fixed << std::setprecision(2) << value / 1000000000000000000.0 << " P";
			return returnString.str();
		}
		returnString << std::fixed << std::setprecision(2) << value / 1000000000000000000000.0 << " Z";
		return returnString.str();
	}

} /* namespace Elpida */
