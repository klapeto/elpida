//
// Created by klapeto on 28/2/2023.
//

#include "Elpida/ValueUtilities.hpp"

#include <iomanip>
#include <sstream>

#include <locale>
#include <codecvt>

namespace Elpida
{
	std::string ValueUtilities::GetValueScaleStringImpl(double value,
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

	std::wstring ValueUtilities::StringToWstring(const std::string& string)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
		return converter.from_bytes(string);
	}

	std::string ValueUtilities::WstringTostring(const std::wstring& wstring)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
		return converter.to_bytes(wstring);
	}
} // Elpida