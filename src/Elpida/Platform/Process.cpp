//
// Created by klapeto on 6/5/2023.
//

#include "Process.hpp"

#include "Elpida/Core/ValueUtilities.hpp"

#include <thread>
#include <sstream>

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

	Process::Process(const String& path, const Vector<String>& args)
			: _path(path), _keepGoing(true)
	{
		_pid = DoStartProcess(path, args, _outputPipe, _errorPipe);

		_stdOutputThread = std::thread(&Process::ReadAllFromPipe, this, &_outputPipe,
				static_cast<std::ostream*>(&_stdOutput));

		_stdErrorThread = std::thread(&Process::ReadAllFromPipe, this, &_errorPipe,
				static_cast<std::ostream*>(&_stdError));
	}

	void Process::WaitToExit()
	{
		WaitToExitImpl(false);
	}

	void Process::ReadAllFromPipe(AnonymousPipe* pipe, std::ostream* stream)
	{
		std::size_t bytesRead = 0;
		char buffer[512];
		while (ReadFromPipe(*pipe, buffer, sizeof(buffer), bytesRead)
			   && _keepGoing.load(std::memory_order_acquire))
		{
			stream->write(buffer, bytesRead);
		}
	}

	Process::~Process()
	{
		_keepGoing.store(false, std::memory_order_release);

		char buff[1]{ 0 };
		std::size_t bytesWriten;
		WriteToPipe(_outputPipe, buff, sizeof(buff), bytesWriten);
		WriteToPipe(_errorPipe, buff, sizeof(buff), bytesWriten);

		_outputPipe.Close();
		_errorPipe.Close();

		if (_stdOutputThread.joinable())
		{
			_stdOutputThread.join();
		}

		if (_stdErrorThread.joinable())
		{
			_stdErrorThread.join();
		}

		WaitToExitImpl(true);
	}

} // Elpida