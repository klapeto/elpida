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

	int Process::DoStartProcess(const String& path,
			const Vector<String>& args,
			AnonymousPipe& outputPipe,
			AnonymousPipe& errorPipe)
	{
		PROCESS_INFORMATION piProcInfo;
		BOOL success = FALSE;

		ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));

		STARTUPINFOW siStartInfo;
		ZeroMemory(&siStartInfo, sizeof(STARTUPINFOW));
		siStartInfo.cb = sizeof(STARTUPINFOW);
		siStartInfo.hStdError = errorPipe.IsOpen() ? errorPipe.GetWriteHandle<HANDLE>() : NULL;
		siStartInfo.hStdOutput = outputPipe.IsOpen() ? outputPipe.GetWriteHandle<HANDLE>() : NULL;
		siStartInfo.hStdInput = NULL;
		siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

		success = CreateProcessW(NULL,
				(LPWSTR)Vu::StringToWstring(GetCommandLine(path, args)).c_str(), // command line
				NULL,                                        // process security attributes
				NULL,                                        // primary thread security attributes
				TRUE,                                        // handles are inherited
				0,                                            // creation flags
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
		auto handle = OpenProcess(SYNCHRONIZE, FALSE, _pid);
		if (handle == nullptr)
		{
			if (noThrow) return;
			throw ElpidaException("Failed to open process: (", _path, ") (", _pid, "): ",
					OsUtilities::GetLastErrorString());
		}

		WaitForSingleObject(handle, INFINITE);

		DWORD exitCode;
		GetExitCodeProcess(handle, &exitCode);

		CloseHandle(handle);

		if (exitCode != 0)
		{
			if (noThrow) return;
			throw ElpidaException("Process exited with error: (", _path, ") (", _pid, "): ");
		}
	}

} // Elpida

#endif // defined(ELPIDA_WINDOWS)