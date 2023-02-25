/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2020  Ioannis Panagiotopoulos
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
// Created by klapeto on 13/9/20.
//

#include "OsUtilities.hpp"

#include <Elpida/Config.hpp>
#include <Elpida/ElpidaException.hpp>
#include <Elpida/Utilities/ValueUtilities.hpp>
#include <string>
#include <sstream>
#include <thread>
#include <iostream>
#include <filesystem>

#ifdef ELPIDA_LINUX

#include "xdg-open.h"
#include <unistd.h>
#include <cstring>
#include <sys/wait.h>

#else
#include <windows.h>
#include <Elpida/Utilities/ValueUtilities.hpp>
#endif

namespace Elpida
{

	void OsUtilities::openUrl(const std::string& url)
	{
#ifdef ELPIDA_LINUX
		auto res = xdg_open(url.c_str());
		if (res)
		{
			throw ElpidaException("Failed to open url", "Status code: " + std::to_string(res));
		}
#else
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
#endif
	}

	static void readAllFromFileDescriptor(int fileDescriptor, std::ostringstream* stream)
	{
		long bytesRead;
		char buffer[512];
		while ((bytesRead = read(fileDescriptor, buffer, sizeof(buffer))) != 0)
		{
			stream->write(buffer, bytesRead);
		}
	}

	static char* convertStringToCharArray(const std::string& str)
	{
		auto size = str.size() + 1;
		auto ptr = new char[size];
		strncpy(ptr, str.c_str(), size);
		return ptr;
	}

	static char** createArgVFromVector(const std::string& filepath, const std::vector<std::string>& args)
	{
		auto argC = args.size() + 2;	// +2, one for the filename, one for the null array terminator
		auto argV = new char* [argC];

		auto filename = std::filesystem::path(filepath)
				.filename()
				.generic_string();

		argV[0] = convertStringToCharArray(filename);	// first argument is the filename

		for (std::size_t i = 0; i < args.size(); ++i)
		{
			argV[i + 1] = convertStringToCharArray(args[i]);
		}

		argV[argC - 1] = nullptr;	// last argument is the array terminator

		return argV;
	}

	static void childProcessProcedure(
			const std::string& path,
			const std::vector<std::string>& args,
			int outputPipe[],
			int errorPipe[]){
		auto argV = createArgVFromVector(path, args);

		if (dup2(outputPipe[1], STDOUT_FILENO) == -1)
		{
			_exit(1);
		}

		if (dup2(errorPipe[1], STDERR_FILENO) == -1)
		{
			_exit(1);
		}

		close(outputPipe[1]);
		close(outputPipe[0]);

		close(errorPipe[1]);
		close(errorPipe[0]);

		if (execvp(path.c_str(), argV) == -1)
		{
			auto err = strerror(errno);
			auto length = strlen(err);
			write(STDERR_FILENO, err, length);	// write the error.
			_exit(1);
		}
	}

	static std::string parentProcessProcedure(
			const std::string& path,
			pid_t pid,
			int outputPipe[],
			int errorPipe[]){

		close(outputPipe[1]);
		close(errorPipe[1]);

		std::ostringstream outputAccumulator;
		std::ostringstream errorAccumulator;

		std::thread outputReaderThread = std::thread(readAllFromFileDescriptor, outputPipe[0], &outputAccumulator);
		std::thread errorReaderThread = std::thread(readAllFromFileDescriptor, errorPipe[0], &errorAccumulator);

		outputReaderThread.join();
		errorReaderThread.join();

		close(outputPipe[0]);
		close(errorPipe[0]);

		int status;
		pid_t waitResult;
		do
		{
			status = 0;
			waitResult = waitpid(pid, &status, 0);
		} while (waitResult == -1 && errno == EINTR);

		if (waitResult == -1)
		{
			throw ElpidaException("executeProcess", Vu::Cs("waitpid() failed: (", path, "): ", strerror(errno)));
		}

		if (!WIFEXITED(status))
		{
			throw ElpidaException("executeProcess", Vu::Cs("child process was killed (", path, ")"));
		}

		if (WEXITSTATUS(status) == 1)
		{
			throw ElpidaException("executeProcess", Vu::Cs("process failed (", path, "): ", errorAccumulator.str()));
		}

		return outputAccumulator.str();
	}


	std::string OsUtilities::executeProcess(const std::string& path, const std::vector<std::string>& args)
	{
#ifdef ELPIDA_LINUX

		int outputPipe[2];
		int errorPipe[2];
		if (pipe(outputPipe) == -1)
		{
			throw ElpidaException("executeProcess", Vu::Cs("output pipe() failed: (", path, "): ", strerror(errno)));
		}

		if (pipe(errorPipe) == -1)
		{
			throw ElpidaException("executeProcess", Vu::Cs("errorPipe pipe() failed: (", path, "): ", strerror(errno)));
		}

		auto pid = fork();
		if (pid == -1)
		{
			throw ElpidaException("executeProcess", Vu::Cs("fork() failed: (", path, "): ", strerror(errno)));
		}

		if (pid == 0)
		{
			childProcessProcedure(path, args, outputPipe, errorPipe);
		}
		else
		{
			return parentProcessProcedure(path, pid, outputPipe, errorPipe);
		}
#else

#endif
	}
}