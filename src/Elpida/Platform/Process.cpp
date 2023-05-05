//
// Created by klapeto on 6/5/2023.
//

#include "Process.hpp"

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

#define  ULONG_BITS  (CHAR_BIT * sizeof (unsigned long))

namespace Elpida
{
	using Vu = Elpida::ValueUtilities;

	std::istream& Process::GetStdOutput()
	{
		return _stdOutput;
	}

	std::istream& Process::GetStdError()
	{
		return _stdError;
	}

	void Process::ReadAllFromFileDescriptor(int fileDescriptor, std::ostream* stream)
	{
		long bytesRead;
		char buffer[512];
		while ((bytesRead = read(fileDescriptor, buffer, sizeof(buffer))) != 0
				&& _keepGoing.load(std::memory_order_acquire))
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

	void Process::ParentProcessProcedure()
	{
		_stdOutputThread = std::thread(&Process::ReadAllFromFileDescriptor, this, _outputPipe[0],
				static_cast<std::ostream*>(&_stdOutput));
		_stdErrorThread = std::thread(&Process::ReadAllFromFileDescriptor, this, _errorPipe[0], static_cast<std::ostream*>(&_stdError));
	}

	Process::Process(const String& path, const Vector<String>& args)
			: _path(path), _keepGoing(true), _outputPipe{ 0, 0 }, _errorPipe{ 0, 0 }
	{
		if (pipe(_outputPipe) == -1)
		{
			throw ElpidaException("ExecuteProcess", Vu::Cs("output pipe() failed: (", path, "): ", strerror(errno)));
		}

		if (pipe(_errorPipe) == -1)
		{
			throw ElpidaException("ExecuteProcess", Vu::Cs("errorPipe pipe() failed: (", path, "): ", strerror(errno)));
		}

		_pid = fork();
		if (_pid == -1)
		{
			throw ElpidaException("ExecuteProcess", Vu::Cs("fork() failed: (", path, "): ", strerror(errno)));
		}

		if (_pid == 0)
		{
			ChildProcessProcedure(path, args, _outputPipe, _errorPipe);
		}
		else
		{
			ParentProcessProcedure();
		}
	}

	void Process::WaitToExit()
	{
		int status;
		pid_t waitResult;
		do
		{
			status = 0;
			waitResult = waitpid(_pid, &status, 0);
		} while (waitResult == -1 && errno == EINTR);

		if (waitResult == -1)
		{
			throw ElpidaException("ExecuteProcess", Vu::Cs("waitpid() failed: (", _path, "): ", strerror(errno)));
		}

		if (!WIFEXITED(status))
		{
			throw ElpidaException("ExecuteProcess", Vu::Cs("child process was killed (", _path, ")"));
		}

		if (WEXITSTATUS(status) == 1)
		{
			throw ElpidaException("ExecuteProcess", Vu::Cs("process failed (", _path, "): ", _stdError.str()));
		}
	}

	Process::~Process()
	{
		_keepGoing.store(false, std::memory_order_release);
		char buff[1]{0};
		write(_outputPipe[0], buff, sizeof(buff));	// Unblock the pipes
		write(_errorPipe[0], buff, sizeof(buff));

		close(_outputPipe[0]);
		close(_errorPipe[0]);
		close(_outputPipe[1]);
		close(_errorPipe[1]);

		if (_stdOutputThread.joinable())
		{
			_stdOutputThread.join();
		}

		if (_stdErrorThread.joinable())
		{
			_stdErrorThread.join();
		}
	}

	void Process::Terminate()
	{
		kill(_pid, SIGTERM);
	}
} // Elpida