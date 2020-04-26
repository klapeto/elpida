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
//	const char* ValueUtilities::PrefixesSI[] = {
//		"",
//		"K",
//		"M",
//		"G",
//		"T",
//		"P",
//		"E",
//		"Z",
//		"Y"
//	};
//
//	const char* ValueUtilities::PrefixesIEC[] = {
//		"",
//		"Ki",
//		"Mi",
//		"Gi",
//		"Ti",
//		"Pi",
//		"Ei",
//		"Zi",
//		"Yi"
//	};
//
//	const double ValueUtilities::ScaleValuesSI[] = {
//		1.0,
//		1000.0,
//		1000.0 * 1000.0,
//		1000.0 * 1000.0 * 1000.0,
//		1000.0 * 1000.0 * 1000.0 * 1000.0,
//		1000.0 * 1000.0 * 1000.0 * 1000.0 * 1000.0,
//		1000.0 * 1000.0 * 1000.0 * 1000.0 * 1000.0 * 1000.0,
//		1000.0 * 1000.0 * 1000.0 * 1000.0 * 1000.0 * 1000.0 * 1000.0,
//		1000.0 * 1000.0 * 1000.0 * 1000.0 * 1000.0 * 1000.0 * 1000.0 * 1000.0
//	};
//
//	const double ValueUtilities::ScaleValuesIEC[] = {
//		1.0,
//		1024.0,
//		1024.0 * 1024.0,
//		1024.0 * 1024.0 * 1024.0,
//		1024.0 * 1024.0 * 1024.0 * 1024.0,
//		1024.0 * 1024.0 * 1024.0 * 1024.0 * 1024.0,
//		1024.0 * 1024.0 * 1024.0 * 1024.0 * 1024.0 * 1024.0,
//		1024.0 * 1024.0 * 1024.0 * 1024.0 * 1024.0 * 1024.0 * 1024.0,
//		1024.0 * 1024.0 * 1024.0 * 1024.0 * 1024.0 * 1024.0 * 1024.0 * 1024.0
//	};

	std::string ValueUtilities::getValueScaleStringImpl(double value, double denominator, const char* prefixes[], size_t arraySize)
	{
		std::ostringstream returnString;
		double accumulator = denominator;
		size_t i = 0;
		while (i < arraySize)
		{
			if (value < accumulator)
			{
				returnString << std::fixed << std::setprecision(2) << value / accumulator * denominator << " "
							 << prefixes[i];
				return returnString.str();
			}

			i++;
			accumulator *= denominator;
		}
	}
} /* namespace Elpida */
