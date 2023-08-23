//
// Created by klapeto on 6/5/2023.
//

#include "Elpida/Core/Config.hpp"

#if defined(ELPIDA_UNIX)

#include "Elpida/Platform/Process.hpp"

#include "Elpida/Core/ElpidaException.hpp"

#include <unistd.h>
#include <sys/wait.h>

#include <sstream>
#include <filesystem>
#include <cerrno>
#include <cstring>
#include <cstdlib>

namespace Elpida
{
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
			AnonymousPipe& outputPipe,
			AnonymousPipe& errorPipe)
	{
		auto argV = CreateArgVFromVector(path, args);

		if (outputPipe.IsOpen() && dup2(outputPipe.GetWriteHandle<int>(), STDOUT_FILENO) == -1)
		{
			_exit(1);
		}

		if (errorPipe.IsOpen() && dup2(errorPipe.GetWriteHandle<int>(), STDERR_FILENO) == -1)
		{
			_exit(1);
		}

		outputPipe.Close();
		errorPipe.Close();

		if (execvp(path.c_str(), argV) == -1)
		{
			auto err = strerror(errno);
			auto length = strlen(err);
			write(STDERR_FILENO, err, length);    // write the error.
		}
		_exit(1);
	}

	int Process::DoStartProcess(const String& path, const Vector<String>& args, AnonymousPipe& outputPipe, AnonymousPipe& errorPipe)
	{
		auto pid = fork();
		if (pid == -1)
		{
			throw ElpidaException("Process start failed: '", path, "'. fork() failed: ", strerror(errno));
		}

		if (pid == 0)
		{
			ChildProcessProcedure(path, args, outputPipe, errorPipe);
		}
		return pid;
	}

	void Process::Terminate()
	{
		if (_pid < 0) return;
		if (kill(_pid, SIGTERM) == -1)
		{
			throw ElpidaException("Failed to terminate process: (", _path, ") (", _pid, "): ", strerror(errno));
		}
	}

	void Process::WaitToExitImpl(bool noThrow) const
	{
		if (_pid < 0) return;
		int status;
		pid_t waitResult;
		do
		{
			status = 0;
			waitResult = waitpid(_pid, &status, 0);
		} while (waitResult == -1 && errno == EINTR);

		if (waitResult == -1)
		{
			if (noThrow) return;
			throw ElpidaException("Wait to exit failed: (", _path, ") (", _pid, "): ", strerror(errno));
		}

		if (!WIFEXITED(status))
		{
			if (noThrow) return;
			throw ElpidaException("Wait to exit failed: (", _path, ") (", _pid, "): Process was killed");
		}

		if (WEXITSTATUS(status) == 1)
		{
			if (noThrow) return;
			throw ElpidaException("Process exited with error: (", _path, ") (", _pid, ")");
		}
	}

} // Elpida


#endif // defined(ELPIDA_UNIX)