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
