//
// Created by klapeto on 12/3/2023.
//

#include "Elpida/Core/Config.hpp"

#if defined(ELPIDA_WINDOWS)

#include "Elpida/Platform/OsUtilities.hpp"

#include "Elpida/Core/ElpidaException.hpp"

#include <thread>
#include <sstream>

#include <windows.h>
#include <strsafe.h>

namespace Elpida
{
	class SafeHandle
	{
	public:

		operator HANDLE() const
		{
			return _handle;
		}

		operator LPHANDLE()
		{
			return &_handle;
		}

		SafeHandle() = default;
		~SafeHandle()
		{
			if (_handle != NULL)
			{
				CloseHandle(_handle);
			}
		}
		SafeHandle(const SafeHandle&) = delete;
		SafeHandle& operator=(const SafeHandle&) = delete;
	private:
		HANDLE _handle = NULL;
	};

	String OsUtilities::GetLastErrorString()
	{
		HRESULT errorId = GetLastError();

		if (errorId == 0) return String();

		LPSTR messageBuffer = nullptr;
		try
		{
			size_t size = FormatMessageA(
					FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL, errorId, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), (LPSTR) & messageBuffer, 0, NULL);

			String message(messageBuffer, size);

			LocalFree(messageBuffer);
			return message;
		}
		catch (...)
		{
			if (messageBuffer != nullptr)
			{
				LocalFree(messageBuffer);
			}

			throw;
		}
	}

	static void ReadAllFromFileDescriptor(HANDLE fileDescriptor, std::ostringstream *stream)
	{
		DWORD bytesRead;
		char buffer[512];

		for (;;)
		{
			auto success = ReadFile(fileDescriptor, buffer, sizeof(buffer), &bytesRead, NULL);
			if (!success || bytesRead == 0)
				break;

			stream->write(buffer, bytesRead);
		}
	}


	String OsUtilities::ExecuteProcess(const String& path, const Vector<String>& args)
	{
		SafeHandle childInPipeRead;
		SafeHandle childInPipeWrite;
		SafeHandle childOutPipeRead;
		SafeHandle childOutPipeWrite;
		SafeHandle childErrPipeRead;
		SafeHandle childErrPipeWrite;

		SECURITY_ATTRIBUTES securityAttributes;
		securityAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
		securityAttributes.bInheritHandle = TRUE;
		securityAttributes.lpSecurityDescriptor = NULL;

		if (!CreatePipe(childOutPipeRead, childOutPipeWrite, &securityAttributes, 0))
		{
			throw ElpidaException("executeProcess", "Failed to create pipe: ");
		}

		if (!SetHandleInformation(childOutPipeRead, HANDLE_FLAG_INHERIT, 0))
		{
			throw ElpidaException("executeProcess", "Failed to set handle information");
		}

		if (!CreatePipe(childInPipeRead, childInPipeWrite, &securityAttributes, 0))
		{
			throw ElpidaException("executeProcess", "Failed to create pipe");
		}

		if (!SetHandleInformation(childInPipeWrite, HANDLE_FLAG_INHERIT, 0))
		{
			throw ElpidaException("executeProcess", "Failed to set handle information");
		}

		if (!CreatePipe(childErrPipeRead, childErrPipeWrite, &securityAttributes, 0))
		{
			throw ElpidaException("executeProcess", "Failed to create pipe");
		}

		if (!SetHandleInformation(childErrPipeWrite, HANDLE_FLAG_INHERIT, 0))
		{
			throw ElpidaException("executeProcess", "Failed to set handle information");
		}

		PROCESS_INFORMATION piProcInfo;
		BOOL success = FALSE;

		ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));

		STARTUPINFOW siStartInfo;
		ZeroMemory(&siStartInfo, sizeof(STARTUPINFOW));
		siStartInfo.cb = sizeof(STARTUPINFOW);
		siStartInfo.hStdError = childErrPipeWrite;
		siStartInfo.hStdOutput = childOutPipeWrite;
		siStartInfo.hStdInput = childInPipeRead;
		siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

		success = CreateProcessW(NULL,
								 (LPWSTR)Vu::stringToWstring(path).c_str(), // command line
								 NULL,										// process security attributes
								 NULL,										// primary thread security attributes
								 TRUE,										// handles are inherited
								 0,											// creation flags
								 NULL,										// use parent's environment
								 NULL,										// use parent's current directory
								 &siStartInfo,								// STARTUPINFO pointer
								 &piProcInfo);								// receives PROCESS_INFORMATION

		// If an error occurs, exit the application.
		if (!success)
		{
			throw ElpidaException("executeProcess", "Failed to create process");
		}

		std::ostringstream outputAccumulator;
		std::ostringstream errorAccumulator;

		std::thread outputReadThread = std::thread(ReadAllFromFileDescriptor, childOutPipeRead, &outputAccumulator);
		std::thread errorReadThread = std::thread(ReadAllFromFileDescriptor, childErrPipeRead, &errorAccumulator);

		outputReadThread.join();
		errorReadThread.join();

		WaitForSingleObject(piProcInfo.hProcess, INFINITE);

		DWORD exitCode;
		GetExitCodeProcess(piProcInfo.hProcess, &exitCode);

		if (exitCode == 0)
		{
			return outputAccumulator.str();
		}

		throw ElpidaException("executeProcess", Vu::Cs("Failed and produced error: ", errorAccumulator.str()));
	}

} // Elpida

#endif // defined(ELPIDA_WINDOWS)