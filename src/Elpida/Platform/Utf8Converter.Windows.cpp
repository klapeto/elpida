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

#include "Elpida/Core/Config.hpp"

#if defined(ELPIDA_WINDOWS)

#include "Utf8Converter.Windows.hpp"

#include "Elpida/Core/ElpidaException.hpp"

#include <windows.h>

namespace Elpida
{
	std::wstring Utf8Converter::ToWide(std::string_view utf8)
	{
		if (utf8.empty()) return {};

		const auto size = MultiByteToWideChar(CP_UTF8, 0, utf8.data(), static_cast<int>(utf8.size()), nullptr, 0);
		if (size <= 0)
		{
			throw ElpidaException("Failed to convert utf8 string to utf16. Error code: ", GetLastError());
		}

		std::wstring wide(static_cast<std::size_t>(size), L'\0');
		MultiByteToWideChar(CP_UTF8, 0, utf8.data(), static_cast<int>(utf8.size()), wide.data(), size);
		return wide;
	}

	std::string Utf8Converter::ToUtf8(std::wstring_view wide)
	{
		if (wide.empty()) return {};

		const auto size = WideCharToMultiByte(CP_UTF8, 0, wide.data(), static_cast<int>(wide.size()), nullptr, 0, nullptr, nullptr);
		if (size <= 0)
		{
			throw ElpidaException("Failed to convert utf16 string to utf8. Error code: ", GetLastError());
		}

		std::string utf8(static_cast<std::size_t>(size), '\0');
		WideCharToMultiByte(CP_UTF8, 0, wide.data(), static_cast<int>(wide.size()), utf8.data(), size, nullptr, nullptr);
		return utf8;
	}

} // Elpida

#endif // defined(ELPIDA_WINDOWS)
