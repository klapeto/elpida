//
// Created by klapeto on 12/3/2023.
//


#include "Elpida/Core/Config.hpp"

#if defined(ELPIDA_UNIX)

#include "Elpida/Platform/OsUtilities.hpp"

#include "Elpida/Core/ValueUtilities.hpp"
#include "Elpida/Core/ElpidaException.hpp"

#include <unistd.h>
#include <sys/wait.h>

#include <thread>
#include <sstream>
#include <filesystem>
#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <climits>

#include <numa.h>

#define  ULONG_BITS  (CHAR_BIT * sizeof (unsigned long))

namespace Elpida
{

	using Vu = Elpida::ValueUtilities;

	String OsUtilities::GetLastErrorString()
	{
		return { strerror(errno) };
	}

	unsigned int OsUtilities::GetNumaNodeIdForProcessor(unsigned int processorId)
	{
		if (numa_available() < 0) return 0;
		return numa_node_of_cpu(processorId);
	}

	static void ReadAllFromFileDescriptor(int fileDescriptor, std::ostringstream* stream)
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

	static char** CreateArgVFromVector(const std::string& filepath, const std::vector<std::string>& args)
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

	[[noreturn]]
	static void ChildProcessProcedure(
			const std::string& path,
			const std::vector<std::string>& args,
			int outputPipe[],
			int errorPipe[])
	{
		auto argV = CreateArgVFromVector(path, args);

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
		}
		_exit(1);
	}

	static std::string ParentProcessProcedure(
			const std::string& path,
			pid_t pid,
			int outputPipe[],
			int errorPipe[])
	{

		close(outputPipe[1]);
		close(errorPipe[1]);

		std::ostringstream outputAccumulator;
		std::ostringstream errorAccumulator;

		std::thread outputReaderThread = std::thread(ReadAllFromFileDescriptor, outputPipe[0], &outputAccumulator);
		std::thread errorReaderThread = std::thread(ReadAllFromFileDescriptor, errorPipe[0], &errorAccumulator);

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
			throw ElpidaException("ExecuteProcess", Vu::Cs("waitpid() failed: (", path, "): ", strerror(errno)));
		}

		if (!WIFEXITED(status))
		{
			throw ElpidaException("ExecuteProcess", Vu::Cs("child process was killed (", path, ")"));
		}

		if (WEXITSTATUS(status) == 1)
		{
			throw ElpidaException("ExecuteProcess", Vu::Cs("process failed (", path, "): ", errorAccumulator.str()));
		}

		return outputAccumulator.str();
	}


	String OsUtilities::ExecuteProcess(const String& path, const Vector<String>& args)
	{
		int outputPipe[2];
		int errorPipe[2];
		if (pipe(outputPipe) == -1)
		{
			throw ElpidaException("ExecuteProcess", Vu::Cs("output pipe() failed: (", path, "): ", strerror(errno)));
		}

		if (pipe(errorPipe) == -1)
		{
			throw ElpidaException("ExecuteProcess", Vu::Cs("errorPipe pipe() failed: (", path, "): ", strerror(errno)));
		}

		auto pid = fork();
		if (pid == -1)
		{
			throw ElpidaException("ExecuteProcess", Vu::Cs("fork() failed: (", path, "): ", strerror(errno)));
		}

		if (pid == 0)
		{
			ChildProcessProcedure(path, args, outputPipe, errorPipe);
		}
		else
		{
			return ParentProcessProcedure(path, pid, outputPipe, errorPipe);
		}
	}

} // Elpida


#endif // defined(ELPIDA_UNIX)