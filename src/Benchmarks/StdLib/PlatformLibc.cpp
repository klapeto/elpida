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

#include "PlatformLibc.hpp"

#if defined(_WIN32)

#include <windows.h>
#include <cctype>
#include <string>

#include "Elpida/Core/ElpidaException.hpp"

namespace Elpida::Benchmarks::StdLib::PlatformLibc
{
	double (__cdecl* StrtodPtr)(const char*, char**) = nullptr;
	unsigned long (__cdecl* StrtoulPtr)(const char*, char**, int) = nullptr;
	void* (__cdecl* MemsetPtr)(void*, int, std::size_t) = nullptr;
	void* (__cdecl* MemchrPtr)(const void*, int, std::size_t) = nullptr;
#if defined(__i386__)
	double (__cdecl* CeilPtr)(double) = nullptr;
	double (__cdecl* FloorPtr)(double) = nullptr;
	double (__cdecl* SqrtPtr)(double) = nullptr;
#else
	float (__cdecl* CeilfPtr)(float) = nullptr;
	float (__cdecl* FloorfPtr)(float) = nullptr;
	float (__cdecl* SqrtfPtr)(float) = nullptr;
#endif

	static std::string GetSelectedCrt()
	{
		char buffer[64];
		DWORD length = GetEnvironmentVariableA("ELPIDA_STDLIB_CRT", buffer, sizeof(buffer));
		if (length == 0)
		{
			return "msvcrt";
		}
		if (length >= sizeof(buffer))
		{
			throw ElpidaException("ELPIDA_STDLIB_CRT is too long. Valid values: msvcrt, ucrt");
		}
		std::string value(buffer, length);
		for (auto& c : value)
		{
			c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
		}
		return value;
	}

	template<typename T>
	static void Resolve(HMODULE module, const char* dll, const char* name, T& pointer)
	{
		auto address = GetProcAddress(module, name);
		if (address == nullptr)
		{
			throw ElpidaException("The C runtime '", dll, "' does not export ", name);
		}
		pointer = reinterpret_cast<T>(reinterpret_cast<void*>(address));
	}

	static const char* DoLoadCrt()
	{
		auto selected = GetSelectedCrt();
		const char* dll;
		if (selected.empty() || selected == "msvcrt")
		{
			dll = "msvcrt.dll";
		}
		else if (selected == "ucrt")
		{
			dll = "ucrtbase.dll";
		}
		else
		{
			throw ElpidaException("Unknown ELPIDA_STDLIB_CRT value '", selected, "'. Valid values: msvcrt, ucrt");
		}

		// Never unloaded: the pointers are used until the process exits
		HMODULE module = LoadLibraryA(dll);
		if (module == nullptr)
		{
			throw ElpidaException("Could not load the C runtime '", dll, "' (ELPIDA_STDLIB_CRT=", selected,
					"). Error code: ", static_cast<unsigned long>(GetLastError()));
		}

		Resolve(module, dll, "strtod", StrtodPtr);
		Resolve(module, dll, "strtoul", StrtoulPtr);
		Resolve(module, dll, "memset", MemsetPtr);
		Resolve(module, dll, "memchr", MemchrPtr);
#if defined(__i386__)
		Resolve(module, dll, "ceil", CeilPtr);
		Resolve(module, dll, "floor", FloorPtr);
		Resolve(module, dll, "sqrt", SqrtPtr);
#else
		Resolve(module, dll, "ceilf", CeilfPtr);
		Resolve(module, dll, "floorf", FloorfPtr);
		Resolve(module, dll, "sqrtf", SqrtfPtr);
#endif
		return dll;
	}

	const char* LoadCrt()
	{
		static const char* loaded = DoLoadCrt(); // thread-safe, once; a failed load throws again on the next call
		return loaded;
	}
}

#endif // defined(_WIN32)
