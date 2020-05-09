//
// Created by klapeto on 8/5/20.
//

#ifndef INCLUDE_ELPIDA_UTILITIES_DURATION_HPP
#define INCLUDE_ELPIDA_UTILITIES_DURATION_HPP

#include <chrono>

namespace Elpida
{
	using NanoSecond = std::nano;
	using MicroSecond = std::micro;
	using MilliSecond = std::milli;
	using Second = std::ratio<1, 1>;
	using Duration = std::chrono::duration<double>;
}

#endif //INCLUDE_ELPIDA_UTILITIES_DURATION_HPP
