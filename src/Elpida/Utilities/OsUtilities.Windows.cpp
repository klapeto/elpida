/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2022  Ioannis Panagiotopoulos
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>
 *************************************************************************/

//
// Created by klapeto on 25/2/2023.
//

#include "Elpida/Config.hpp"

#if defined(ELPIDA_WINDOWS)

#include "Elpida/Utilities/OsUtilities.hpp"
#include "Elpida/Utilities/ValueUtilities.hpp"
#include "Elpida/ElpidaException.hpp"

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


	std::string OsUtilities::GetErrorString(HRESULT errorId)
	{
		LPSTR messageBuffer = nullptr;
		try
		{
			if (errorId == 0) return std::string();

			size_t size = FormatMessageA(
					FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL, errorId, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), (LPSTR) & messageBuffer, 0, NULL);

			std::string message(messageBuffer, size);

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

	std::string OsUtilities::ReadRegistryKeyFromHKLM(const std::string& subKey, const std::string& key)
	{
		// Modified version of https://stackoverflow.com/a/50821858
		auto regSubKey = Vu::stringToWstring(subKey);
		auto regValue = Vu::stringToWstring(key);
		size_t bufferSize = 0xFFF; // If too small, will be resized down below.
		std::wstring valueBuf; // Contiguous buffer since C++11.
		valueBuf.resize(bufferSize);
		auto cbData = static_cast<DWORD>(bufferSize * sizeof(wchar_t));
		HRESULT rc = RegGetValueW(
				HKEY_LOCAL_MACHINE,
				regSubKey.c_str(),
				regValue.c_str(),
				RRF_RT_REG_SZ,
				nullptr,
				static_cast<void*>(valueBuf.data()),
				&cbData
		);
		while (rc == ERROR_MORE_DATA)
		{
			// Get a buffer that is big enough.
			cbData /= sizeof(wchar_t);
			if (cbData > static_cast<DWORD>(bufferSize))
			{
				bufferSize = static_cast<size_t>(cbData);
			}
			else
			{
				bufferSize *= 2;
				cbData = static_cast<DWORD>(bufferSize * sizeof(wchar_t));
			}
			valueBuf.resize(bufferSize);
			rc = RegGetValueW(
					HKEY_LOCAL_MACHINE,
					regSubKey.c_str(),
					regValue.c_str(),
					RRF_RT_REG_SZ,
					nullptr,
					static_cast<void*>(valueBuf.data()),
					&cbData
			);
		}
		if (rc == ERROR_SUCCESS)
		{
			cbData /= sizeof(wchar_t);
			valueBuf.resize(static_cast<size_t>(cbData - 1)); // remove end null character
			return Vu::wstringTostring(valueBuf);
		}
		else
		{
			throw ElpidaException(FUNCTION_NAME,
					Vu::Cs("Failed to get Registry key: '", subKey, "' and value: '", key, "'. Reason: ",
							GetErrorString(rc)));
		}
	}

	std::string OsUtilities::GetLastErrorString()
	{
		return GetErrorString(GetLastError());
	}

	void OsUtilities::setCurrentThreadAffinity(unsigned int cpuId)
	{
		if (!SetThreadAffinityMask(GetCurrentThread(), 1 << (int)cpuId))
		{
			throw ElpidaException(FUNCTION_NAME,
					Vu::Cs("Failed to set thread affinity to: ", cpuId, ". Error: ", GetLastErrorString()));
		}
	}

	static void readAllFromFileDescriptor(HANDLE fileDescriptor, std::ostringstream *stream)
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

	std::string OsUtilities::executeProcess(const std::string &path, const std::vector<std::string> &args)
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

		std::thread outputReadThread = std::thread(readAllFromFileDescriptor, childOutPipeRead, &outputAccumulator);
		std::thread errorReadThread = std::thread(readAllFromFileDescriptor, childErrPipeRead, &errorAccumulator);

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

	void OsUtilities::openUrl(const std::string& url)
	{
		auto res = ShellExecuteW(NULL, L"open", Vu::stringToWstring(url).c_str(), NULL, NULL, SW_SHOWDEFAULT);
		if (res <= (HINSTANCE)32)
		{
			std::string error;
			switch ((intptr_t)res)
			{
			case 0:
				error = "The operating system is out of memory or resources.";
				break;
			case ERROR_FILE_NOT_FOUND:
				error = "The specified file was not found.";
				break;
			case ERROR_PATH_NOT_FOUND:
				error = "The specified path was not found.";
				break;
			case ERROR_BAD_FORMAT:
				error = "The .exe file is invalid (non-Win32 .exe or error in .exe image).";
				break;
			case SE_ERR_ACCESSDENIED:
				error = "The operating system denied access to the specified file.";
				break;
			case SE_ERR_ASSOCINCOMPLETE:
				error = "The file name association is incomplete or invalid.";
				break;
			case SE_ERR_DDEBUSY:
				error =
					"The DDE transaction could not be completed because other DDE transactions were being processed.";
				break;
			case SE_ERR_DDEFAIL:
				error = "The DDE transaction failed.";
				break;
			case SE_ERR_DDETIMEOUT:
				error = "The DDE transaction could not be completed because the request timed out.";
				break;
			case SE_ERR_DLLNOTFOUND:
				error = "The specified DLL was not found.";
				break;
			case SE_ERR_NOASSOC:
				error =
					"There is no application associated with the given file name extension. This error will also be returned if you attempt to print a file that is not printable.";
				break;
			case SE_ERR_OOM:
				error = "There was not enough memory to complete the operation.";
				break;
			case SE_ERR_SHARE:
				error = "A sharing violation occurred. ";
				break;
			default:
				error = "Unknown Error";
				break;
			}

			throw ElpidaException("Failed to open url", error);
		}
	}
}

#endif