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