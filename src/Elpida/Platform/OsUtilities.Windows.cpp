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

#include "Elpida/Core/Config.hpp"

#if defined(ELPIDA_WINDOWS)

#include "Elpida/Platform/OsUtilities.hpp"
#include "Elpida/Core/ElpidaException.hpp"
#include "Elpida/Core/ValueUtilities.hpp"
#include "Utf8Converter.Windows.hpp"

#include <windows.h>
#include <strsafe.h>

#include <cstring>
#include <iostream>
#include <streambuf>

namespace Elpida
{
	// Writes utf8 text to a Windows console with WriteConsoleW, so it is displayed
	// correctly regardless of the console code page (which is usually the OEM one).
	class ConsoleUTF8StreamBuffer final : public std::streambuf
	{
	 public:
		explicit ConsoleUTF8StreamBuffer(HANDLE handle)
				: _handle(handle)
		{
			setp(_buffer, _buffer + sizeof(_buffer));
		}

	 protected:
		int_type overflow(int_type ch) override
		{
			if (Flush() != 0) return traits_type::eof();

			if (!traits_type::eq_int_type(ch, traits_type::eof()))
			{
				*pptr() = traits_type::to_char_type(ch);
				pbump(1);
			}
			return traits_type::not_eof(ch);
		}

		int sync() override
		{
			return Flush();
		}

	 private:
		HANDLE _handle;
		char _buffer[1024];

		// Returns the length up to the last complete utf8 sequence, so we never split a character between writes.
		static std::size_t GetCompleteUTF8Length(const char* data, std::size_t size)
		{
			for (std::size_t i = 1; i <= 3 && i <= size; ++i)
			{
				const auto c = static_cast<unsigned char>(data[size - i]);
				if ((c & 0xC0) == 0x80) continue;	// continuation byte

				const std::size_t expected = c >= 0xF0 ? 4 : c >= 0xE0 ? 3 : c >= 0xC0 ? 2 : 1;
				return expected > i ? size - i : size;
			}
			return size;
		}

		int Flush()
		{
			const auto size = static_cast<std::size_t>(pptr() - pbase());
			const auto completeSize = GetCompleteUTF8Length(pbase(), size);

			if (completeSize > 0)
			{
				std::wstring wide;
				try
				{
					wide = Utf8Converter::ToWide({ pbase(), completeSize });
				}
				catch (...)
				{
					return -1;
				}

				const auto wideSize = static_cast<DWORD>(wide.size());
				DWORD offset = 0;
				while (offset < wideSize)
				{
					DWORD written = 0;
					if (!WriteConsoleW(_handle, wide.data() + offset, wideSize - offset, &written, nullptr)
						|| written == 0)
					{
						return -1;
					}
					offset += written;
				}
			}

			const auto remaining = size - completeSize;
			std::memmove(_buffer, pbase() + completeSize, remaining);
			setp(_buffer, _buffer + sizeof(_buffer));
			pbump(static_cast<int>(remaining));
			return 0;
		}
	};

	static void RedirectStreamToConsole(std::ostream& stream, DWORD stdHandle)
	{
		const auto handle = GetStdHandle(stdHandle);
		DWORD mode;
		if (handle == nullptr || handle == INVALID_HANDLE_VALUE || !GetConsoleMode(handle, &mode))
		{
			// Redirected to a file/pipe: keep writing raw utf8 bytes
			return;
		}

		stream.flush();
		// Intentionally never deleted: std streams are used (and flushed) until the very end of the process
		stream.rdbuf(new ConsoleUTF8StreamBuffer(handle));
	}

	void OsUtilities::SetupConsoleUTF8Output()
	{
		RedirectStreamToConsole(std::cout, STD_OUTPUT_HANDLE);
		RedirectStreamToConsole(std::cerr, STD_ERROR_HANDLE);
		RedirectStreamToConsole(std::clog, STD_ERROR_HANDLE);
	}

	unsigned int OsUtilities::GetNumaNodeIdForProcessor(unsigned int processorId)
	{
		UCHAR NodeNumber;

		GetNumaProcessorNode(static_cast<UCHAR>(processorId), &NodeNumber);	// TODO: does not work with 64+ processors (thanks windows)
		return NodeNumber;
	}

	bool OsUtilities::ConvertArgumentsToUTF8(int& originalArgC, char**& originalArgV)
	{
		// based on https://github.com/HandBrake/HandBrake/blame/master/test/test.c
#if defined( __MINGW32__ )
		bool ret = false;
		int argc = 0;
		char **argv = nullptr;

		auto **argVUtf16 = CommandLineToArgvW(GetCommandLineW(), &argc);
		if (argVUtf16 != nullptr)
		{
			int offset = (argc + 1) * sizeof(char*);
			int size = offset;

			for(auto i = 0; i < argc; i++)
			{
				size += WideCharToMultiByte(CP_UTF8, 0, argVUtf16[i], -1, nullptr, 0, nullptr, nullptr );
			}

			argv = static_cast<char **>(malloc(size));
			if (argv != nullptr)
			{
				for (auto i = 0; i < argc; i++)
				{
					argv[i] = reinterpret_cast<char *>(argv) + offset;
					offset += WideCharToMultiByte(CP_UTF8, 0, argVUtf16[i], -1, argv[i], size - offset, nullptr, nullptr);
				}
				argv[argc] = nullptr;
				ret = true;
			}
			LocalFree(argVUtf16);
		}
		if (ret)
		{
			originalArgC = argc;
			originalArgV = argv;
		}
		return ret;
#else
		// On other systems, assume command line is already utf8
		return true;
#endif
	}

	static std::string GetErrorString(DWORD errorId)
	{
		if (errorId == 0) return {};

		LPWSTR messageBuffer = nullptr;
		const auto size = FormatMessageW(
				FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				nullptr,
				errorId,
				0,	// Let Windows pick: neutral -> thread -> user -> system -> en-US
				reinterpret_cast<LPWSTR>(&messageBuffer),
				0,
				nullptr);

		if (size == 0) return ValueUtilities::Cs("Error getting string of the error: ", std::to_string(GetLastError()) , " (original error: " ,std::to_string(errorId), ")");

		std::string message;
		try
		{
			message = Utf8Converter::ToUtf8({ messageBuffer, static_cast<std::size_t>(size) });
		}
		catch (...)
		{
			LocalFree(messageBuffer);
			throw;
		}
		LocalFree(messageBuffer);

		// System messages end with "\r\n"
		while (!message.empty() && (message.back() == '\n' || message.back() == '\r' || message.back() == ' '))
		{
			message.pop_back();
		}
		return message;
	}

	std::string OsUtilities::GetLastErrorString()
	{
		return GetErrorString(GetLastError());
	}

	std::filesystem::path OsUtilities::GetExecutableDirectory()
	{
		return GetExecutablePath().parent_path();
	}

	std::filesystem::path OsUtilities::GetExecutablePath()
	{
		wchar_t buffer[MAX_PATH];

		// The size is in characters, not bytes
		const auto length = GetModuleFileNameW(nullptr, buffer, MAX_PATH);
		if (length == MAX_PATH || GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			throw ElpidaException("Failed to get the executable path. Path required larger buffer");
		}
		// Construct from the native string directly to avoid any narrow string encoding guesses
		return std::filesystem::path(std::wstring(buffer, length));
	}

	std::string OsUtilities::ReadRegistryKeyFromHKLM(const std::string& subKey, const std::string& key)
	{
		const auto regSubKey = Utf8Converter::ToWide(subKey);
		const auto regValue = Utf8Converter::ToWide(key);

		HKEY hKey = {};
		auto result = RegOpenKeyExW(HKEY_LOCAL_MACHINE, regSubKey.c_str(), 0, KEY_READ, &hKey);
		if (result != ERROR_SUCCESS)
		{
			// Reg* functions return the error code instead of setting the last error
			throw ElpidaException("Failed to get Registry key: '", subKey, "' and value: '", key, "'. Reason: ", GetErrorString(result));
		}

		std::wstring valueBuf(0xFFF, L'\0');
		DWORD cbData;
		do
		{
			cbData = static_cast<DWORD>(valueBuf.size() * sizeof(wchar_t));
			result = RegQueryValueExW(hKey,
					regValue.c_str(),
					nullptr,
					nullptr,
					reinterpret_cast<LPBYTE>(valueBuf.data()),
					&cbData);

			if (result == ERROR_MORE_DATA)
			{
				// cbData now holds the required size in bytes
				valueBuf.resize(cbData / sizeof(wchar_t) + 1);
			}
		} while (result == ERROR_MORE_DATA);

		RegCloseKey(hKey);

		if (result != ERROR_SUCCESS)
		{
			throw ElpidaException("Failed to get Registry key: '", subKey, "' and value: '", key, "'. Reason: ", GetErrorString(result));
		}

		valueBuf.resize(cbData / sizeof(wchar_t));
		// String values may or may not be stored with the null terminator
		while (!valueBuf.empty() && valueBuf.back() == L'\0')
		{
			valueBuf.pop_back();
		}

		return Utf8Converter::ToUtf8(valueBuf);
	}

} // Elpida

#endif // defined(ELPIDA_WINDOWS)