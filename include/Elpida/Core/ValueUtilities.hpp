//
// Created by klapeto on 28/2/2023.
//

#ifndef ELPIDA_VALUEUTILITIES_HPP_
#define ELPIDA_VALUEUTILITIES_HPP_

#include "Elpida/Core/Size.hpp"
#include "Elpida/Core/String.hpp"

#include <sstream>
#include <iomanip>
#include <locale>
#include <limits>
#include <chrono>

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

		enum class TimeValues
		{
			PicoSeconds,
			NanoSeconds,
			MicroSeconds,
			MilliSeconds,
			Seconds,
			Minutes,
			Hours,
			Days,
			Months,
			Years
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

		static inline const char* TimeSuffixes[(int)TimeValues::Years + 1] = {
				"ps",
				"ns",
				"μs",
				"ms",
				"seconds",
				"minutes",
				"hours",
				"days",
				"months",
				"years"
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

		static inline const double TimeValues[] = {
				1.0 / 1000.0 / 1000.0 / 1000.0 / 1000.0,
				1.0 / 1000.0 / 1000.0 / 1000.0,
				1.0 / 1000.0 / 1000.0,
				1.0 / 1000.0,
				1.0,
				60.0,
				60.0 * 60.0,
				60.0 * 60.0 * 24.0,
				60.0 * 60.0 * 24.0 * 30.0,
				60.0 * 60.0 * 24.0 * 365
		};

		template<class T, Size N>
		static constexpr Size GetArrayLength(T (&)[N])
		{
			return N;
		}

		template<typename T>
		static String ToFixed(T value, int decimals)
		{
			std::ostringstream returnString;
			returnString << std::fixed << std::setprecision(decimals) << value;
			return returnString.str();
		}

		static inline String ToSI(double value, int decimals = 2)
		{
			return GetValueScaleStringSI(value, decimals);
		}

		static inline String ToIEC(double value, int decimals = 2)
		{
			return GetValueScaleStringIEC(value, decimals);
		}

		static String GetValueScaleStringSI(double value, int decimals = 2)
		{
			if (value <= std::numeric_limits<double>::epsilon()) return "0";
			return GetValueScaleStringImpl(value, ScaleValuesSI, PrefixesSI, GetArrayLength(PrefixesSI), decimals);
		}

		static String GetValueScaleStringIEC(double value, int decimals = 2)
		{
			return GetValueScaleStringImpl(value, ScaleValuesIEC, PrefixesIEC, GetArrayLength(PrefixesIEC), decimals);
		}

		template<typename T>
		static std::string GetTimeScaleValue(std::chrono::duration<T> duration, int decimals = 2)
		{
			return GetValueScaleStringImpl(duration.count(), TimeValues, TimeSuffixes, GetArrayLength(TimeSuffixes),
					decimals);
		}

		template<typename ... TArgs>
		inline static String ConcatenateToString(TArgs&& ... args)
		{
			std::ostringstream stream;
			concatenateToStringImpl(stream, std::forward<TArgs>(args)...);
			return stream.str();
		}

		template<typename ... TArgs>
		inline static String Cs(TArgs&& ... args)
		{
			return ConcatenateToString(std::forward<TArgs>(args)...);
		}

		inline static String Trim(const String& input, char c = ' ')
		{
			const Size size = input.size();

			Size startCount = 0;
			while (input[startCount] == c) startCount++;

			Size endCount = 1;
			while (size - endCount > startCount && input[size - endCount] == c) endCount++;

			--endCount;

			return input.substr(startCount, size - endCount - startCount);
		}

		static String DoubleToStringInvariant(double d)
		{
			std::ostringstream doubleToStringAccumulator;
			doubleToStringAccumulator.imbue(std::locale::classic());
			doubleToStringAccumulator << d;
			return doubleToStringAccumulator.str();
		}

		static double StringToDoubleInvariant(const String& str)
		{
			const std::string oldLocale = std::setlocale(LC_NUMERIC, nullptr);
			std::setlocale(LC_NUMERIC, "C");
			try
			{
				double value = std::stod(str);
				std::setlocale(LC_NUMERIC, oldLocale.c_str());
				return value;
			}
			catch (...)
			{
				std::setlocale(LC_NUMERIC, oldLocale.c_str());
				throw;
			}
		}

		static String WstringTostring(const WString& wstring);

		static WString StringToWstring(const String& string);

		static void QuoteString(std::string& str)
		{
			if (str.empty()) return;

			if (str.front() != '"')
			{
				str.insert(0, "\"");
			}

			if (str.back() != '"')
			{
				str.insert(str.size(), "\"");
			}
		}

		static void DeQuoteString(std::string& str)
		{
			if (str.empty()) return;
			if (str.front() == '"')
			{
				str.erase(0, 1);
			}
			if (str.back() == '"')
			{
				str.erase(str.size() - 1, 1);
			}
		}

	private:
		static String GetValueScaleStringImpl(double value,
				const double denominators[],
				const char* prefixes[],
				Size arraySize,
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

} // Elpida

#endif //ELPIDA_VALUEUTILITIES_HPP_
