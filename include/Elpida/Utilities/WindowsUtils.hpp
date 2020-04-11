//
// Created by klapeto on 11/4/20.
//

#ifndef INCLUDE_ELPIDA_UTILITIES_WINDOWSUTILS_HPP
#define INCLUDE_ELPIDA_UTILITIES_WINDOWSUTILS_HPP

#include "Elpida/Config.hpp"

namespace Elpida
{
	class WindowsUtils
	{
	public:

#ifdef ELPIDA_WINDOWS
		static std::string GetLastErrorString();
#endif
		WindowsUtils() = delete;
	};
}


#endif //INCLUDE_ELPIDA_UTILITIES_WINDOWSUTILS_HPP
