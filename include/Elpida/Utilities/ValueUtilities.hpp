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
			Pico,
			Nano,
			Micro,
			Milli,
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

		static inline const char* PrefixesSI[(int)SIPrefixes::Yotta + 1] = {
			"p",
			"n",
			"μ",
			"m",
			"",
			"K",
			"M",
			"G",
			"T",
			"P",
			"E",
			"Z",
			"Y"
		};

		static inline const char* PrefixesIEC[(int)IECPrefixes::Yobi + 1] = {
			"",
			"Ki",
			"Mi",
			"Gi",
			"Ti",
			"Pi",
			"Ei",
			"Zi",
			"Yi"
		};

		static inline const double ScaleValuesSI[] = {
			1.0 / 1000.0 / 1000.0 / 1000.0 / 1000.0,
			1.0 / 1000.0 / 1000.0 / 1000.0,
			1.0 / 1000.0 / 1000.0,
			1.0 / 1000.0,
			1.0,
			1000.0,
			1000.0 * 1000.0,
			1000.0 * 1000.0 * 1000.0,
			1000.0 * 1000.0 * 1000.0 * 1000.0,
			1000.0 * 1000.0 * 1000.0 * 1000.0 * 1000.0,
			1000.0 * 1000.0 * 1000.0 * 1000.0 * 1000.0 * 1000.0,
			1000.0 * 1000.0 * 1000.0 * 1000.0 * 1000.0 * 1000.0 * 1000.0,
			1000.0 * 1000.0 * 1000.0 * 1000.0 * 1000.0 * 1000.0 * 1000.0 * 1000.0
		};

		static inline const double ScaleValuesIEC[] = {
			1.0,
			1024.0,
			1024.0 * 1024.0,
			1024.0 * 1024.0 * 1024.0,
			1024.0 * 1024.0 * 1024.0 * 1024.0,
			1024.0 * 1024.0 * 1024.0 * 1024.0 * 1024.0,
			1024.0 * 1024.0 * 1024.0 * 1024.0 * 1024.0 * 1024.0,
			1024.0 * 1024.0 * 1024.0 * 1024.0 * 1024.0 * 1024.0 * 1024.0,
			1024.0 * 1024.0 * 1024.0 * 1024.0 * 1024.0 * 1024.0 * 1024.0 * 1024.0
		};

		static inline constexpr double SIDenominator = 1000.0;
		static inline constexpr double IECDenominator = 1024.0;

		template<class T, size_t N>
		static constexpr size_t getArrayLength(T (&)[N])
		{
			return N;
		}

		static std::string getValueScaleStringSI(double value, int decimals = 2)
		{
			return getValueScaleStringImpl(value, ScaleValuesSI, PrefixesSI, getArrayLength(PrefixesSI), decimals);
		}

		static std::string getValueScaleStringIEC(double value, int decimals = 2)
		{
			return getValueScaleStringImpl(value, ScaleValuesIEC, PrefixesIEC, getArrayLength(PrefixesIEC), decimals);
		}

		template<typename ... TArgs>
		inline static std::string concatenateToString(TArgs&& ... args)
		{
			std::ostringstream stream;
			concatenateToStringImpl(stream, std::forward<TArgs>(args)...);
			return stream.str();
		}

		template<typename ... TArgs>
		inline static std::string Cs(TArgs&& ... args)
		{
			return concatenateToString(std::forward<TArgs>(args)...);
		}

	private:
		static std::string getValueScaleStringImpl(double value,
			const double denominators[],
			const char* prefixes[],
			size_t arraySize,
			int decimals);

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
