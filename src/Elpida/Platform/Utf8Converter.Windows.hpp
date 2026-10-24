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
// Created by klapeto on 24/10/2026.
//

#ifndef ELPIDA_PLATFORM_UTF8CONVERTER_WINDOWS_HPP
#define ELPIDA_PLATFORM_UTF8CONVERTER_WINDOWS_HPP

#include "Elpida/Core/Config.hpp"

#if defined(ELPIDA_WINDOWS)

#include <string>
#include <string_view>

namespace Elpida
{
	// Internally all strings are utf8. Windows "W" APIs use utf16.
	// Convert only at the WinAPI boundary with these.
	class Utf8Converter
	{
	 public:
		static std::wstring ToWide(std::string_view utf8);
		static std::string ToUtf8(std::wstring_view wide);
	};

} // Elpida

#endif // defined(ELPIDA_WINDOWS)

#endif //ELPIDA_PLATFORM_UTF8CONVERTER_WINDOWS_HPP
