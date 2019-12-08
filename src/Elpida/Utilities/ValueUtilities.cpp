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
 * ValueUtilities.cpp
 *
 *  Created on: 19 Οκτ 2018
 *      Author: klapeto
 */

#include "Elpida/Utilities/ValueUtilities.hpp"

#include <iomanip>
#include <ratio>
#include <sstream>

namespace Elpida
{
	std::string ValueUtilities::getValueScaleString(double value)
	{
		std::ostringstream returnString;
		if (value < std::kilo::num)
		{
			returnString << std::fixed << std::setprecision(2) << value;
			return returnString.str();
		}

		if (value < std::mega::num)
		{
			returnString << std::fixed << std::setprecision(2) << value / std::kilo::num << " K";
			return returnString.str();
		}

		if (value < std::giga::num)
		{
			returnString << std::fixed << std::setprecision(2) << value / std::mega::num << " M";
			return returnString.str();
		}

		if (value < std::tera::num)
		{
			returnString << std::fixed << std::setprecision(2) << value / std::giga::num << " G";
			return returnString.str();
		}

		if (value < std::peta::num)
		{
			returnString << std::fixed << std::setprecision(2) << value / std::tera::num << " T";
			return returnString.str();
		}

		if (value < std::exa::num)
		{
			returnString << std::fixed << std::setprecision(2) << value / std::peta::num << " P";
			return returnString.str();
		}
		returnString << std::fixed << std::setprecision(2) << value / std::exa::num << " E";
		return returnString.str();
	}
} /* namespace Elpida */
