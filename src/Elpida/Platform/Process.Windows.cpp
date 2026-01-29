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
// Created by klapeto on 6/5/2023.
//
#include "Elpida/Core/Config.hpp"

#if defined(ELPIDA_WINDOWS)

#include "Elpida/Platform/Process.hpp"

#include "Elpida/Core/ElpidaException.hpp"
#include "Elpida/Core/ValueUtilities.hpp"

#include "Elpida/Platform/OsUtilities.hpp"

#include <sstream>
#include <windows.h>

namespace Elpida
{
	using Vu = ValueUtilities;

	static
#if UNICODE
	std::wstring
#else
	std::string
#endif
	GetCommandLineString(const std::filesystem::path& path, const Vector<String>& args)
	{
#if UNICODE
		std::wostringstream accumulator;
		accumulator << path.native();
#else
		std::ostringstream accumulator;
		accumulator << path.string();
#endif

		for (auto& arg: args)
		{
			accumulator << " " << OsUtilities::GetWinApiStringFromString(arg);
		}

		return accumulator.str();
	}

	int Process::DoStartProcess(const std::filesystem::path& path,
			const Vector<String>& args,
			AnonymousPipe& outputPipe,
			AnonymousPipe& errorPipe)
	{
		PROCESS_INFORMATION piProcInfo;
		BOOL success = FALSE;

		auto actualPath = path;
		actualPath.replace_extension(std::filesystem::path("exe"));	// We are passed unix like with no extension

		ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));

		STARTUPINFO siStartInfo;
		ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
		siStartInfo.cb = sizeof(STARTUPINFO);
		siStartInfo.hStdError = errorPipe.IsOpen() ? errorPipe.GetWriteHandle<HANDLE>() : nullptr;
		siStartInfo.hStdOutput = outputPipe.IsOpen() ? outputPipe.GetWriteHandle<HANDLE>() : nullptr;
		siStartInfo.hStdInput = nullptr;
		siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

		auto cmd = GetCommandLineString(actualPath, args);
		success = CreateProcess(
				nullptr,							// application name
				const_cast<LPTSTR>(cmd.c_str()),	// command line
				nullptr,							// process security attributes
				nullptr,							// primary thread security attributes
				TRUE,								// handles are inherited
				CREATE_NO_WINDOW,					// creation flags
				nullptr,							// use parent's environment
				nullptr,							// use parent's current directory
				&siStartInfo,						// STARTUPINFO pointer
				&piProcInfo);						// receives PROCESS_INFORMATION

		// If an error occurs, exit the application.
		if (!success)
		{
			throw ElpidaException("Failed to create process: ", OsUtilities::GetLastErrorString());
		}

		CloseHandle(piProcInfo.hProcess);
		CloseHandle(piProcInfo.hThread);

		return static_cast<int>(piProcInfo.dwProcessId);
	}

	void Process::Terminate()
	{
		if (_pid < 0) return;
		auto handle = OpenProcess(PROCESS_TERMINATE, FALSE, _pid);
		if (handle == nullptr)
		{
			throw ElpidaException("Failed to terminate process: (", _path, ") (", _pid, "): ",
					OsUtilities::GetLastErrorString());
		}

		if (!TerminateProcess(handle, 1))
		{
			auto errorStr = OsUtilities::GetLastErrorString();
			CloseHandle(handle);
			throw ElpidaException("Failed to terminate process: (", _path, ") (", _pid, "): ", errorStr);
		}
		CloseHandle(handle);
	}

	void Process::WaitToExitImpl(bool noThrow) const
	{
		if (_pid < 0) return;
		auto flags = SYNCHRONIZE |
#if _WIN32_WINNT >= _WIN32_WINNT_WIN6
				PROCESS_QUERY_LIMITED_INFORMATION;
#else
				PROCESS_QUERY_INFORMATION;	// windows XP quirk
#endif
		auto handle = OpenProcess(SYNCHRONIZE | PROCESS_QUERY_INFORMATION, FALSE, _pid);
		if (handle == nullptr)
		{
			// Assume exited.
			return;
#if false
			if (noThrow) return;
			throw ElpidaException("Failed to open process: (", _path, ") (", _pid, "): ",
					OsUtilities::GetLastErrorString());
#endif
		}

		if (WaitForSingleObject(handle, INFINITE) > 0)
		{
			CloseHandle(handle);
			if (noThrow) return;
			throw ElpidaException("Failed to wait for process: ", OsUtilities::GetLastErrorString());
		}

		DWORD exitCode;
		if (GetExitCodeProcess(handle, &exitCode) == 0)
		{
			CloseHandle(handle);
			if (noThrow) return;
			throw ElpidaException("Failed to get process exit code: ", OsUtilities::GetLastErrorString());
		}

		CloseHandle(handle);

		if (exitCode != 0)
		{
			if (noThrow) return;
			throw ElpidaException("Process exited with error: (", _path, ") (", _pid, "): ");
		}
	}

} // Elpida

#endif // defined(ELPIDA_WINDOWS)