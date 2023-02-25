/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2020  Ioannis Panagiotopoulos
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

//
// Created by klapeto on 21/6/20.
//

#ifndef INCLUDE_ELPIDA_UTILITIES_OSUTILITIES_HPP
#define INCLUDE_ELPIDA_UTILITIES_OSUTILITIES_HPP

#include "Elpida/Config.hpp"

#ifdef ELPIDA_WINDOWS
#define WINVER 0x0601
#define _WIN32_WINNT 0x0601
#include <windows.h>
#endif
#include <string>
#include <vector>

namespace Elpida
{
	class OsUtilities
	{
	public:
		static void setCurrentThreadAffinity(unsigned int cpuId);

		static std::string GetLastErrorString();

		static void openUrl(const std::string& url);
		static std::string executeProcess(const std::string& path, const std::vector<std::string>& args);

#ifdef ELPIDA_WINDOWS
		static std::string GetErrorString(HRESULT errorId);
		static std::string ReadRegistryKeyFromHKLM(const std::string& regSubKey, const std::string& regValue);
#else
		static std::string readSysfsString(const std::string& path);
		static double readSysfsValue(const std::string& path);
#endif
		OsUtilities() = delete;
	};
}


#endif //INCLUDE_ELPIDA_UTILITIES_OSUTILITIES_HPP
