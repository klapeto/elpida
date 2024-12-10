//
// Created by klapeto on 6/5/2023.
//
#include "Elpida/Core/Config.hpp"

#if defined(ELPIDA_WINDOWS)

#include "Elpida/Platform/Process.hpp"

#include "Elpida/Core/ElpidaException.hpp"
#include "Elpida/Core/ValueUtilities.hpp"

#include "Elpida/Platform/OsUtilities.hpp"

#include <thread>
#include <sstream>

#include <windows.h>

namespace Elpida
{
	using Vu = ValueUtilities;

	static String GetCommandLine(const String& path, const Vector<String>& args)
	{
		std::ostringstream accumulator;

		accumulator << path;

		for (auto& arg: args)
		{
			accumulator << " " << arg;
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
		siStartInfo.hStdError = errorPipe.IsOpen() ? errorPipe.GetWriteHandle<HANDLE>() : NULL;
		siStartInfo.hStdOutput = outputPipe.IsOpen() ? outputPipe.GetWriteHandle<HANDLE>() : NULL;
		siStartInfo.hStdInput = NULL;
		siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

		success = CreateProcess(NULL,
				(LPSTR)GetCommandLine(actualPath.string(), args).c_str(), // command line
				NULL,                                        // process security attributes
				NULL,                                        // primary thread security attributes
				TRUE,                                        // handles are inherited
				CREATE_NO_WINDOW,                                            // creation flags
				NULL,                                        // use parent's environment
				NULL,                                        // use parent's current directory
				&siStartInfo,                                // STARTUPINFO pointer
				&piProcInfo);                                // receives PROCESS_INFORMATION

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