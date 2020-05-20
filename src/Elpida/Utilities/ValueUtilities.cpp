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
#include <sstream>

namespace Elpida
{

	std::string ValueUtilities::getValueScaleStringImpl(double value,
		const double denominators[],
		const char* prefixes[],
		size_t arraySize,
		int decimals)
	{
		std::ostringstream returnString;
		size_t i = arraySize - 1;
		while (i > 0)
		{
			if (value >= denominators[i])
			{
				break;
			}
			i--;
		}
		returnString << std::fixed << std::setprecision(decimals) << value / denominators[i] << " "
					 << prefixes[i];
		return returnString.str();
	}
} /* namespace Elpida */
