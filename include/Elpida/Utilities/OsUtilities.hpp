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
#include <string>

namespace Elpida
{
	struct OsInfo
	{
		std::string category;
		std::string name;
		std::string version;
	};

	class OsUtilities
	{
	public:

		static OsInfo getOsInfo();

#ifdef ELPIDA_WINDOWS
		static std::string GetLastErrorString();
		static std::wstring ReadRegistryKeyFromHKLM(const std::wstring& regSubKey, const std::wstring& regValue);
#endif
		OsUtilities() = delete;
	};
}


#endif //INCLUDE_ELPIDA_UTILITIES_OSUTILITIES_HPP
