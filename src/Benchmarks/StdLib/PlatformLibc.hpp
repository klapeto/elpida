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

#ifndef ELPIDA_BENCHMARKS_STDLIB_PLATFORMLIBC_HPP
#define ELPIDA_BENCHMARKS_STDLIB_PLATFORMLIBC_HPP

// The StdLib benchmarks measure the platform's C library (docs/development/code-parity.md#the-stdlib-benchmarks).
// Call the C functions, not libc++'s std::floor/ceil/sqrt: those use __builtin_* which the compiler may inline or
// fold, skipping the library.
//
// Windows: the functions are taken from the C runtime DLL at runtime (LoadCrt()), selected with the environment
// variable ELPIDA_STDLIB_CRT:
//   msvcrt (default, or unset)  msvcrt.dll, the system C runtime on every Windows version, XP included
//   ucrt                        ucrtbase.dll, the Universal CRT (Windows 10+, or installed by an update or a
//                               Visual C++ 2015+ redistributable on older versions)
// Calls through these pointers compile to the same `call *slot` as a dllimport call. Taking them from the DLL also
// bypasses mingw-w64's own implementations, which would otherwise win at link time: libmsvcrt.a defines ceilf, floorf
// and sqrtf as static code, and stdlib.h turns strtod into an inline wrapper around its gdtoa (__mingw_strtod).
// Only the measured functions switch; the process (startup, stdio, the allocator) stays on msvcrt.
// i386: neither msvcrt.dll nor ucrtbase.dll exports ceilf, floorf and sqrtf, so the float functions call ceil, floor
// and sqrt with a conversion to double and back, as MSVC's x86 <math.h> does.

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <math.h>

namespace Elpida::Benchmarks::StdLib::PlatformLibc
{
#if defined(_WIN32)
	// Set by LoadCrt()
	extern double (__cdecl* StrtodPtr)(const char*, char**);
	extern unsigned long (__cdecl* StrtoulPtr)(const char*, char**, int);
	extern void* (__cdecl* MemsetPtr)(void*, int, std::size_t);
	extern void* (__cdecl* MemchrPtr)(const void*, int, std::size_t);
#if defined(__i386__)
	extern double (__cdecl* CeilPtr)(double);
	extern double (__cdecl* FloorPtr)(double);
	extern double (__cdecl* SqrtPtr)(double);
#else
	extern float (__cdecl* CeilfPtr)(float);
	extern float (__cdecl* FloorfPtr)(float);
	extern float (__cdecl* SqrtfPtr)(float);
#endif

	/// Loads the C runtime selected by ELPIDA_STDLIB_CRT, once. Throws ElpidaException if the value is unknown or
	/// the DLL or one of the functions is missing. Returns the DLL's name.
	const char* LoadCrt();

	inline double Strtod(const char* str, char** end) { return StrtodPtr(str, end); }
	inline unsigned long Strtoul(const char* str, char** end, int base) { return StrtoulPtr(str, end, base); }
	inline void* Memset(void* dest, int value, std::size_t size) { return MemsetPtr(dest, value, size); }
	inline const void* Memchr(const void* src, int value, std::size_t size) { return MemchrPtr(src, value, size); }
#if defined(__i386__)
	inline float Ceilf(float x) { return static_cast<float>(CeilPtr(x)); }
	inline float Floorf(float x) { return static_cast<float>(FloorPtr(x)); }
	inline float Sqrtf(float x) { return static_cast<float>(SqrtPtr(x)); }
#else
	inline float Ceilf(float x) { return CeilfPtr(x); }
	inline float Floorf(float x) { return FloorfPtr(x); }
	inline float Sqrtf(float x) { return SqrtfPtr(x); }
#endif
#else
	inline const char* LoadCrt() { return "libc"; }

	inline double Strtod(const char* str, char** end) { return ::strtod(str, end); }
	inline unsigned long Strtoul(const char* str, char** end, int base) { return ::strtoul(str, end, base); }
	inline void* Memset(void* dest, int value, std::size_t size) { return ::memset(dest, value, size); }
	inline const void* Memchr(const void* src, int value, std::size_t size) { return ::memchr(src, value, size); }
	inline float Ceilf(float x) { return ::ceilf(x); }
	inline float Floorf(float x) { return ::floorf(x); }
	inline float Sqrtf(float x) { return ::sqrtf(x); }
#endif
}

#endif //ELPIDA_BENCHMARKS_STDLIB_PLATFORMLIBC_HPP
