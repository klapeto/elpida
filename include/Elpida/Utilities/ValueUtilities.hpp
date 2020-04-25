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
 * ValueUtilities.hpp
 *
 *  Created on: 19 Οκτ 2018
 *      Author: klapeto
 */

#ifndef ELPIDA_UTILITIES_VALUEUTILITIES_HPP_
#define ELPIDA_UTILITIES_VALUEUTILITIES_HPP_

#include <string>
#include <sstream>

namespace Elpida
{

	class ValueUtilities
	{
	public:
		ValueUtilities() = delete;
		~ValueUtilities() = delete;

		enum class SIPrefixes
		{
			None,
			Kilo,
			Mega,
			Giga,
			Tera,
			Peta,
			Exa,
			Zetta,
			Yotta
		};

		enum class IECPrefixes
		{
			None,
			Kibi,
			Mebi,
			Gibi,
			Tebi,
			Pebi,
			Exbi,
			Zebi,
			Yobi
		};

		static const char* PrefixesSI[(int)SIPrefixes::Yotta + 1];
		static const char* PrefixesIEC[(int)IECPrefixes::Yobi + 1];

		static const double ScaleValuesSI[(int)SIPrefixes::Yotta + 1];
		static const double ScaleValuesIEC[(int)IECPrefixes::Yobi + 1];

		static std::string getValueScaleStringSI(double value)
		{
			return getValueScaleStringImpl(value, 1000);
		}

		static std::string getValueScaleStringIEC(double value)
		{
			return getValueScaleStringImpl(value, 1024);
		}

		template<typename ... TArgs>
		inline static std::string concatenateToString(TArgs&& ... args)
		{
			std::ostringstream stream;
			concatenateToStringImpl(stream, std::forward<TArgs>(args)...);
			return stream.str();
		}
	private:
		static std::string getValueScaleStringImpl(double value, double denominator);

		template<typename T, typename ... TRest>
		inline static void concatenateToStringImpl(std::ostringstream& str, T first, TRest&& ... rest)
		{
			str << first;
			concatenateToStringImpl(str, std::forward<TRest>(rest)...);
		}

		template<typename T>
		inline static void concatenateToStringImpl(std::ostringstream&)
		{

		}

		template<typename T>
		inline static void concatenateToStringImpl(std::ostringstream& str, T arg)
		{
			str << arg;
		}

	};
	using Vu = ValueUtilities;
} /* namespace Elpida */

#endif /* ELPIDA_UTILITIES_VALUEUTILITIES_HPP_ */
