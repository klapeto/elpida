//
// Created by klapeto on 6/5/2023.
//

#include "Elpida/Platform/Process.hpp"

#include "Elpida/Core/ValueUtilities.hpp"
#include "Elpida/Core/ElpidaException.hpp"

namespace Elpida
{
	using Vu = Elpida::ValueUtilities;

	Process::Process()
			:_pid(-1)
	{

	}

	Process::Process(const std::filesystem::path& path, const Vector<String>& args, bool redirectStdOut,
			bool redirectStdErr)
			:_path(path)
	{
		if (is_directory(path))
		{
			throw ElpidaException("Process must be called with a file, not directories etc.");
		}

		if (redirectStdOut)
		{
			_outputPipe.Open();
		}

		if (redirectStdErr)
		{
			_errorPipe.Open();
		}
		_pid = DoStartProcess(std::filesystem::absolute(path), args, _outputPipe, _errorPipe);
	}

	void Process::WaitToExit() const
	{
		WaitToExitImpl(false);
	}

	Process::~Process()
	{
		if (_pid < 0) return;
		char buffer[1]{ 0 };
		if (_outputPipe.IsOpen())
		{
			_outputPipe.Write(buffer, sizeof(buffer));
		}

		if (_errorPipe.IsOpen())
		{
			_errorPipe.Write(buffer, sizeof(buffer));
		}


		WaitToExitImpl(true);
	}

	AnonymousPipe& Process::GetStdOut()
	{
		return _outputPipe;
	}

	AnonymousPipe& Process::GetStdErr()
	{
		return _errorPipe;
	}

	Process::Process(Process&& other) noexcept
	{
		_outputPipe = std::move(other._outputPipe);
		_errorPipe = std::move(other._errorPipe);
		_path = std::move(other._path);
		_pid = other._pid;
		other._pid = -1;
	}

	Process& Process::operator=(Process&& other) noexcept
	{
		_outputPipe = std::move(other._outputPipe);
		_errorPipe = std::move(other._errorPipe);
		_path = std::move(other._path);
		_pid = other._pid;
		other._pid = -1;
		return *this;
	}

} // Elpida