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

#if defined(ELPIDA_UNIX)

#include "Elpida/Utilities/OsUtilities.hpp"

#include "Elpida/Utilities/ValueUtilities.hpp"
#include "Elpida/Common/ElpidaException.hpp"

#include <numa.h>

#include <thread>
#include <filesystem>
#include <fstream>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <fcntl.h>

#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <climits>

#define  ULONG_BITS  (CHAR_BIT * sizeof (unsigned long))

namespace Elpida
{

	std::string OsUtilities::GetLastErrorString()
	{
		return { strerror(errno) };
	}

	void OsUtilities::setCurrentThreadAffinity(unsigned int cpuId)
	{
		cpu_set_t mask;
		CPU_ZERO(&mask);
		CPU_SET(cpuId, &mask);

		if (sched_setaffinity(0, sizeof(cpu_set_t), &mask))
		{
			throw ElpidaException(FUNCTION_NAME,
					Vu::Cs("Failed to set thread affinity to: ", cpuId, ". Error: ", GetLastErrorString()));
		}
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
		auto argC = args.size() + 2;    // +2, one for the filename, one for the null array terminator
		auto argV = new char* [argC];

		auto filename = std::filesystem::path(filepath)
				.filename()
				.generic_string();

		argV[0] = convertStringToCharArray(filename);    // first argument is the filename

		for (std::size_t i = 0; i < args.size(); ++i)
		{
			argV[i + 1] = convertStringToCharArray(args[i]);
		}

		argV[argC - 1] = nullptr;    // last argument is the array terminator

		return argV;
	}

	static void childProcessProcedure(
			const std::string& path,
			const std::vector<std::string>& args,
			int outputPipe[],
			int errorPipe[])
	{
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
			write(STDERR_FILENO, err, length);    // write the error.
			_exit(1);
		}
	}

	static std::string parentProcessProcedure(
			const std::string& path,
			pid_t pid,
			int outputPipe[],
			int errorPipe[])
	{

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
	}

	static inline int redirectToDevNull(const int fd)
	{
		int tempFd;
		do
		{
			tempFd = open("/dev/null", O_RDWR | O_NOCTTY);
		} while (tempFd == -1 && errno == EINTR);

		if (tempFd == -1)
		{
			return errno;
		}

		if (tempFd != fd)
		{
			if (dup2(tempFd, fd) == -1)
			{
				const int saved_errno = errno;
				close(tempFd);
				return errno = saved_errno;
			}
			if (close(tempFd) == -1)
			{
				return errno;
			}
		}

		return 0;
	}

	static void closeAllFileDescriptors()
	{
		int limit = (int)sysconf(_SC_OPEN_MAX);
		for (int i = STDERR_FILENO + 1; i < limit; ++i)
		{
			close(i);
		}
	}

	void OsUtilities::openUrl(const std::string& url)
	{
		auto pid = fork();
		if (pid == -1)
		{
			throw ElpidaException("executeProcess", Vu::Cs("fork() failed: (xdg-open): ", GetLastErrorString()));
		}

		if (pid == 0)
		{
			auto uid = getuid();    // Real, not effective, user.
			auto gid = getgid();    // Real, not effective, group.

			/* Close all open file descriptors. */
			closeAllFileDescriptors();

			/* Redirect standard streams, if possible. */
			redirectToDevNull(STDIN_FILENO);
			redirectToDevNull(STDOUT_FILENO);
			redirectToDevNull(STDERR_FILENO);

			if (setresgid(gid, gid, gid) == -1
				|| setresuid(uid, uid, uid) == -1)
			{
				_exit(98);
			}

			setsid();    // Have the child process execute in a new process group.

			auto argV = createArgVFromVector("xdg-open", { url });

			if (execvp("xdg-open", argV) == -1)
			{
				_exit(99);
			}
		}
		else
		{
			pid_t result;
			auto status = 0;
			do
			{
				result = waitpid(pid, &status, 0);
			} while (result == -1 && errno == EINTR);

			if (result == -1)
			{
				throw ElpidaException(FUNCTION_NAME, Vu::Cs("wait() failed: (xdg-open): ", GetLastErrorString()));
			}

			if (!WIFEXITED(status))
			{
				throw ElpidaException(FUNCTION_NAME, Vu::Cs("(xdg-open) was killed: "));
			}

			switch (WEXITSTATUS(status))
			{
			case 0:
				return;
			case 1:
				throw ElpidaException(FUNCTION_NAME, Vu::Cs("(xdg-open) invalid argument: ", url));
			case 2:
				throw ElpidaException(FUNCTION_NAME, Vu::Cs("(xdg-open) No such file or directory : ", url));
			case 3:
				throw ElpidaException(FUNCTION_NAME, Vu::Cs("(xdg-open) Not implemented procedure : ", url));
			case 4:
				throw ElpidaException(FUNCTION_NAME, Vu::Cs("(xdg-open) protocol error : ", url));
			case 98:
				throw ElpidaException(FUNCTION_NAME, Vu::Cs("(xdg-open) permission denied : ", url));
			case 99:
				throw ElpidaException(FUNCTION_NAME, Vu::Cs("(xdg-open) is not installed : ", url));
			default:
				throw ElpidaException(FUNCTION_NAME,
						Vu::Cs("(xdg-open) unknown error (", WEXITSTATUS(status), "): ", url));
			}
		}
	}

	template<typename T>
	static T readSysfs(const std::string& path)
	{
		std::fstream file(path, std::fstream::in);
		file.exceptions(std::ios_base::failbit);
		T value{};
		if (file.good())
		{
			file >> value;
		}
		return value;
	}

	double OsUtilities::readSysfsValue(const std::string& path)
	{
		return readSysfs<double>(path);
	}

	std::string OsUtilities::readSysfsString(const std::string& path)
	{
		return readSysfs<std::string>(path);
	}

}
#endif