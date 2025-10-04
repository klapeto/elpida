/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

//
// Created by klapeto on 12/3/2023.
//

#ifndef ELPIDA_OSUTILITIES_HPP_
#define ELPIDA_OSUTILITIES_HPP_

#include "Elpida/Core/Config.hpp"
#include "Elpida/Core/String.hpp"
#include <filesystem>

#if defined(ELPIDA_WINDOWS)
#include <windows.h>
#endif

namespace Elpida
{

	class OsUtilities
	{
	 public:
		static String GetLastErrorString();
		static std::filesystem::path GetExecutableDirectory();
		static std::filesystem::path GetExecutablePath();
		static unsigned int GetNumaNodeIdForProcessor(unsigned int processorId);

		static bool ConvertArgumentsToUTF8(int& originalArgC, char**& originalArgV);

#if defined(ELPIDA_WINDOWS)
		static std::string GetStringFromWinApiBuffer(LPCTSTR buffer, DWORD size);

		static
#if UNICODE
		std::wstring
#else
		std::string
#endif
		GetWinApiStringFromString(const std::string& str);

		static std::string ReadRegistryKeyFromHKLM(const std::string& subKey, const std::string& key);
#endif
	};

} // Elpida

#endif //ELPIDA_OSUTILITIES_HPP_
