/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

//
// Created by klapeto on 28/2/2023.
//

#include "Elpida/Core/ValueUtilities.hpp"

#include <iomanip>
#include <sstream>

#include <locale>
#include <codecvt>

namespace Elpida
{
	String ValueUtilities::GetValueScaleStringImpl(double value,
		const double denominators[],
		const char* prefixes[],
		Size arraySize,
		int decimals)
	{
		std::ostringstream returnString;
		Size i = arraySize - 1;
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

	WString ValueUtilities::StringToWstring(const String& string)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
		return converter.from_bytes(string);
	}

	String ValueUtilities::WstringTostring(const WString& wstring)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
		return converter.to_bytes(wstring);
	}
} // Elpida