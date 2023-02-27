//
// Created by klapeto on 27/2/2023.
//

#ifndef ELPIDA_DURATION_H
#define ELPIDA_DURATION_H

#include <chrono>

namespace Elpida
{
	using TimeStamp = std::chrono::time_point<std::chrono::high_resolution_clock>;
	using NanoSeconds = std::chrono::duration<double, std::nano>;
	using MicroSeconds = std::chrono::duration<double, std::micro>;
	using MilliSeconds = std::chrono::duration<double, std::milli>;
	using Seconds = std::chrono::duration<double, std::ratio<1, 1>>;
	using Duration = Seconds;

	using Timer = std::chrono::high_resolution_clock;

	template<typename T>
	constexpr Duration ToDuration(const T& duration)
	{
		return std::chrono::duration_cast<Duration>(duration);
	}

	template<typename TR, typename T>
	constexpr TR DurationCast(const T& duration)
	{
		return std::chrono::duration_cast<T>(duration);
	}

}

#endif //ELPIDA_DURATION_H
