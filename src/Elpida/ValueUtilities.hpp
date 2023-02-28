//
// Created by klapeto on 28/2/2023.
//

#ifndef _VALUEUTILITIES_HPP_
#define _VALUEUTILITIES_HPP_

#include <cstdlib>
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

		template<class T, std::size_t N>
		static constexpr std::size_t GetArrayLength(T (&)[N])
		{
			return N;
		}

		static std::string GetValueScaleStringSI(double value, int decimals = 2)
		{
			return GetValueScaleStringImpl(value, ScaleValuesSI, PrefixesSI, GetArrayLength(PrefixesSI), decimals);
		}

		static std::string GetValueScaleStringIEC(double value, int decimals = 2)
		{
			return GetValueScaleStringImpl(value, ScaleValuesIEC, PrefixesIEC, GetArrayLength(PrefixesIEC), decimals);
		}

		template<typename ... TArgs>
		inline static std::string ConcatenateToString(TArgs&& ... args)
		{
			std::ostringstream stream;
			concatenateToStringImpl(stream, std::forward<TArgs>(args)...);
			return stream.str();
		}

		template<typename ... TArgs>
		inline static std::string Cs(TArgs&& ... args)
		{
			return ConcatenateToString(std::forward<TArgs>(args)...);
		}

		static std::string WstringTostring(const std::wstring& wstring);
		static std::wstring StringToWstring(const std::string& string);

	 private:
		static std::string GetValueScaleStringImpl(double value,
			const double denominators[],
			const char* prefixes[],
			size_t arraySize,
			int decimals);

		template<typename T, typename ... TRest>
		inline static void ConcatenateToStringImpl(std::ostringstream& str, T first, TRest&& ... rest)
		{
			str << first;
			ConcatenateToStringImpl(str, std::forward<TRest>(rest)...);
		}

		template<typename T>
		inline static void ConcatenateToStringImpl(std::ostringstream&)
		{

		}

		template<typename T>
		inline static void ConcatenateToStringImpl(std::ostringstream& str, T arg)
		{
			str << arg;
		}
	};

} // Elpida

#endif //_VALUEUTILITIES_HPP_
