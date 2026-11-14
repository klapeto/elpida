/*
 *  Copyright (c) 2026  Ioannis Panagiotopoulos
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

#include "Elpida/Core/Config.hpp"

#if defined(ELPIDA_WINDOWS)

#include "Elpida/Platform/VirtualizationDetector.hpp"
#include "VirtualizationDetector.Common.hpp"

#include <windows.h>

#include <cstdint>
#include <string>
#include <vector>

namespace Elpida
{
	using namespace Virtualization;

	/// String number `index` (1-based) of the SMBIOS structure whose strings start at `strings`; "" if absent.
	static std::string GetSmbiosString(const std::uint8_t* strings, const std::uint8_t* end, std::uint8_t index)
	{
		if (index == 0)
		{
			return {};
		}
		auto current = strings;
		for (std::uint8_t i = 1; current < end && *current != 0; ++i)
		{
			auto stringEnd = current;
			while (stringEnd < end && *stringEnd != 0)
			{
				++stringEnd;
			}
			if (i == index)
			{
				return { reinterpret_cast<const char*>(current), static_cast<std::size_t>(stringEnd - current) };
			}
			current = stringEnd + 1;
		}
		return {};
	}

	/// The VM that the SMBIOS System Information (type 1) manufacturer and product name belong to, or "".
	/// GetSystemFirmwareTable exists from Windows Vista (and XP x64), so it's looked up at runtime: Windows XP x86
	/// relies on CPUID alone.
	static std::string DetectFromSmbios()
	{
		using GetSystemFirmwareTableFunction = UINT (WINAPI*)(DWORD, DWORD, PVOID, DWORD);
		auto kernel32 = GetModuleHandleW(L"kernel32.dll");
		if (kernel32 == nullptr)
		{
			return {};
		}
		auto getSystemFirmwareTable = reinterpret_cast<GetSystemFirmwareTableFunction>(
				reinterpret_cast<void*>(GetProcAddress(kernel32, "GetSystemFirmwareTable")));
		if (getSystemFirmwareTable == nullptr)
		{
			return {};
		}

		constexpr DWORD Rsmb = 'R' << 24 | 'S' << 16 | 'M' << 8 | 'B';
		auto size = getSystemFirmwareTable(Rsmb, 0, nullptr, 0);
		if (size == 0)
		{
			return {};
		}
		std::vector<std::uint8_t> data(size);
		if (getSystemFirmwareTable(Rsmb, 0, data.data(), size) != size)
		{
			return {};
		}

		// RawSMBIOSData: 4 version bytes, a DWORD table length, then the table
		constexpr std::size_t headerSize = 8;
		if (size < headerSize)
		{
			return {};
		}
		std::uint32_t tableLength;
		std::memcpy(&tableLength, data.data() + 4, sizeof(tableLength));
		auto current = data.data() + headerSize;
		auto end = current + (tableLength < size - headerSize ? tableLength : size - headerSize);

		// Each structure: type, formatted length, handle, the formatted area, then NUL-terminated strings ending
		// with an extra NUL
		while (current + 4 <= end)
		{
			auto type = current[0];
			auto length = current[1];
			if (length < 4 || current + length > end)
			{
				break;
			}
			auto strings = current + length;
			auto next = strings;
			while (next + 1 < end && (next[0] != 0 || next[1] != 0))
			{
				++next;
			}
			next += 2;

			if (type == 1 && length >= 6)
			{
				// System Information: manufacturer (offset 4) and product name (offset 5) string numbers
				return DetectFromFirmware(GetSmbiosString(strings, end, current[4]),
						GetSmbiosString(strings, end, current[5]));
			}
			if (type == 127)
			{
				break;
			}
			current = next;
		}
		return {};
	}

	String VirtualizationDetector::Detect()
	{
#if defined(ELPIDA_VIRTUALIZATION_CPUID)
		auto fromCpuid = DetectFromCpuid();
		if (!fromCpuid.empty())
		{
			return fromCpuid;
		}
#endif
		auto fromSmbios = DetectFromSmbios();
		if (!fromSmbios.empty())
		{
			return fromSmbios;
		}
		return "none";
	}
} // Elpida

#endif // defined(ELPIDA_WINDOWS)
